//===- CgLiveRegMatrix.cpp - Track register interference
//--------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CgLiveRegMatrix analysis pass.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/live_reg_matrix.h"
#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/pass/live_interval.h"
#include "compiler/cgir/pass/live_interval_union.h"
#include "compiler/cgir/pass/live_intervals.h"
#include "compiler/cgir/pass/register_coalescer.h"
#include "compiler/cgir/pass/virt_reg_map.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/LaneBitmask.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>

using namespace llvm;

namespace COMPILER {

CgLiveRegMatrix::CgLiveRegMatrix(CgFunction &MF) {
  runOnCgFunction(MF);
  MF.Matrix = this;
}
bool CgLiveRegMatrix::runOnCgFunction(CgFunction &MF) {
  TRI = MF.getSubtarget().getRegisterInfo();
  LIS = MF.LIS;
  VRM = MF.VRM;

  unsigned NumRegUnits = TRI->getNumRegUnits();
  if (NumRegUnits != Matrix.size())
    Queries.reset(new CgLiveIntervalUnion::Query[NumRegUnits]);
  Matrix.init(LIUAlloc, NumRegUnits);

  // Make sure no stale queries get reused.
  invalidateVirtRegs();
  return false;
}

void CgLiveRegMatrix::releaseMemory() {
  for (unsigned i = 0, e = Matrix.size(); i != e; ++i) {
    Matrix[i].clear();
    // No need to clear Queries here, since CgLiveIntervalUnion::Query
    // doesn't have anything important to clear and CgLiveRegMatrix's
    // runOnFunction() does a std::unique_ptr::reset anyways.
  }
}

template <typename Callable>
static bool foreachUnit(const TargetRegisterInfo *TRI,
                        const CgLiveInterval &VRegInterval, MCRegister PhysReg,
                        Callable Func) {
  if (VRegInterval.hasSubRanges()) {
    for (MCRegUnitMaskIterator Units(PhysReg, TRI); Units.isValid(); ++Units) {
      unsigned Unit = (*Units).first;
      LaneBitmask Mask = (*Units).second;
      for (const CgLiveInterval::SubRange &S : VRegInterval.subranges()) {
        if ((S.LaneMask & Mask).any()) {
          if (Func(Unit, S))
            return true;
          break;
        }
      }
    }
  } else {
    for (MCRegUnitIterator Units(PhysReg, TRI); Units.isValid(); ++Units) {
      if (Func(*Units, VRegInterval))
        return true;
    }
  }
  return false;
}

void CgLiveRegMatrix::assign(const CgLiveInterval &VirtReg,
                             MCRegister PhysReg) {
  assert(!VRM->hasPhys(VirtReg.reg()) && "Duplicate VirtReg assignment");
  VRM->assignVirt2Phys(VirtReg.reg(), PhysReg);

  foreachUnit(TRI, VirtReg, PhysReg,
              [&](unsigned Unit, const CgLiveRange &Range) {
                Matrix[Unit].unify(VirtReg, Range);
                return false;
              });
}

void CgLiveRegMatrix::unassign(const CgLiveInterval &VirtReg) {
  Register PhysReg = VRM->getPhys(VirtReg.reg());
  VRM->clearVirt(VirtReg.reg());

  foreachUnit(TRI, VirtReg, PhysReg,
              [&](unsigned Unit, const CgLiveRange &Range) {
                Matrix[Unit].extract(VirtReg, Range);
                return false;
              });
}

bool CgLiveRegMatrix::isPhysRegUsed(MCRegister PhysReg) const {
  for (MCRegUnitIterator Unit(PhysReg, TRI); Unit.isValid(); ++Unit) {
    if (!Matrix[*Unit].empty())
      return true;
  }
  return false;
}

bool CgLiveRegMatrix::checkRegMaskInterference(const CgLiveInterval &VirtReg,
                                               MCRegister PhysReg) {
  // Check if the cached information is valid.
  // The same BitVector can be reused for all PhysRegs.
  // We could cache multiple VirtRegs if it becomes necessary.
  if (RegMaskVirtReg != VirtReg.reg() || RegMaskTag != UserTag) {
    RegMaskVirtReg = VirtReg.reg();
    RegMaskTag = UserTag;
    RegMaskUsable.clear();
    LIS->checkRegMaskInterference(VirtReg, RegMaskUsable);
  }

  // The BitVector is indexed by PhysReg, not register unit.
  // Regmask interference is more fine grained than regunits.
  // For example, a Win64 call can clobber %ymm8 yet preserve %xmm8.
  return !RegMaskUsable.empty() && (!PhysReg || !RegMaskUsable.test(PhysReg));
}

bool CgLiveRegMatrix::checkRegUnitInterference(const CgLiveInterval &VirtReg,
                                               MCRegister PhysReg) {
  if (VirtReg.empty())
    return false;
  CgCoalescerPair CP(VirtReg.reg(), PhysReg, *TRI);

  bool Result = foreachUnit(
      TRI, VirtReg, PhysReg, [&](unsigned Unit, const CgLiveRange &Range) {
        const CgLiveRange &UnitRange = LIS->getRegUnit(Unit);
        return Range.overlaps(UnitRange, CP, *LIS->getSlotIndexes());
      });
  return Result;
}

CgLiveIntervalUnion::Query &CgLiveRegMatrix::query(const CgLiveRange &LR,
                                                   MCRegister RegUnit) {
  CgLiveIntervalUnion::Query &Q = Queries[RegUnit];
  Q.init(UserTag, LR, Matrix[RegUnit]);
  return Q;
}

CgLiveRegMatrix::InterferenceKind
CgLiveRegMatrix::checkInterference(const CgLiveInterval &VirtReg,
                                   MCRegister PhysReg) {
  if (VirtReg.empty())
    return IK_Free;

  // Regmask interference is the fastest check.
  if (checkRegMaskInterference(VirtReg, PhysReg))
    return IK_RegMask;

  // Check for fixed interference.
  if (checkRegUnitInterference(VirtReg, PhysReg))
    return IK_RegUnit;

  // Check the matrix for virtual register interference.
  bool Interference = foreachUnit(TRI, VirtReg, PhysReg,
                                  [&](MCRegister Unit, const CgLiveRange &LR) {
                                    return query(LR, Unit).checkInterference();
                                  });
  if (Interference)
    return IK_VirtReg;

  return IK_Free;
}

bool CgLiveRegMatrix::checkInterference(CgSlotIndex Start, CgSlotIndex End,
                                        MCRegister PhysReg) {
  // Construct artificial live range containing only one segment [Start, End).
  CgVNInfo valno(0, Start);
  CgLiveRange::Segment Seg(Start, End, &valno);
  CgLiveRange LR;
  LR.addSegment(Seg);

  // Check for interference with that segment
  for (MCRegUnitIterator Units(PhysReg, TRI); Units.isValid(); ++Units) {
    // LR is stack-allocated. CgLiveRegMatrix caches queries by a key that
    // includes the address of the live range. If (for the same reg unit)
    // this checkInterference overload is called twice, without any other
    // query() calls in between (on heap-allocated LiveRanges)  - which
    // would invalidate the cached query - the LR address seen the second
    // time may well be the same as that seen the first time, while the
    // Start/End/valno may not - yet the same cached result would be
    // fetched. To avoid that, we don't cache this query.
    //
    // FIXME: the usability of the Query API needs to be improved to avoid
    // subtle bugs due to query identity. Avoiding caching, for example,
    // would greatly simplify things.
    CgLiveIntervalUnion::Query Q;
    Q.reset(UserTag, LR, Matrix[*Units]);
    if (Q.checkInterference())
      return true;
  }
  return false;
}

Register CgLiveRegMatrix::getOneVReg(unsigned PhysReg) const {
  const CgLiveInterval *VRegInterval = nullptr;
  for (MCRegUnitIterator Unit(PhysReg, TRI); Unit.isValid(); ++Unit) {
    if ((VRegInterval = Matrix[*Unit].getOneVReg()))
      return VRegInterval->reg();
  }

  return MCRegister::NoRegister;
}

} // namespace COMPILER
