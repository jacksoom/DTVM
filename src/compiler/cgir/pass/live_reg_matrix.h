//===- CgLiveRegMatrix.h - Track register interference ----------*- C++
//-*---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The CgLiveRegMatrix analysis pass keeps track of virtual register
// interference along two dimensions: Slot indexes and register units. The
// matrix is used by register allocators to ensure that no interfering virtual
// registers get assigned to overlapping physical registers.
//
// Register units are defined in MCRegisterInfo.h, they represent the smallest
// unit of interference when dealing with overlapping physical registers. The
// CgLiveRegMatrix is represented as a CgLiveIntervalUnion per register unit.
// When a virtual register is assigned to a physical register, the live range
// for the virtual register is inserted into the CgLiveIntervalUnion for each
// regunit in the physreg.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/live_interval_union.h"
#include "llvm/ADT/BitVector.h"
#include <memory>

namespace COMPILER {

class CgLiveInterval;
class CgLiveIntervals;
class CgFunction;
class CgVirtRegMap;

class CgLiveRegMatrix : public NonCopyable {
  const TargetRegisterInfo *TRI;
  CgLiveIntervals *LIS;
  CgVirtRegMap *VRM;

  // UserTag changes whenever virtual registers have been modified.
  unsigned UserTag = 0;

  // The matrix is represented as a CgLiveIntervalUnion per register unit.
  CgLiveIntervalUnion::Allocator LIUAlloc;
  CgLiveIntervalUnion::Array Matrix;

  // Cached queries per register unit.
  std::unique_ptr<CgLiveIntervalUnion::Query[]> Queries;

  // Cached register mask interference info.
  unsigned RegMaskTag = 0;
  unsigned RegMaskVirtReg = 0;
  BitVector RegMaskUsable;

  // MachineFunctionPass boilerplate.
  bool runOnCgFunction(CgFunction &);
  void releaseMemory();

public:
  CgLiveRegMatrix(CgFunction &MF);
  ~CgLiveRegMatrix() { releaseMemory(); }
  //===--------------------------------------------------------------------===//
  // High-level interface.
  //===--------------------------------------------------------------------===//
  //
  // Check for interference before assigning virtual registers to physical
  // registers.
  //

  /// Invalidate cached interference queries after modifying virtual register
  /// live ranges. Interference checks may return stale information unless
  /// caches are invalidated.
  void invalidateVirtRegs() { ++UserTag; }

  enum InterferenceKind {
    /// No interference, go ahead and assign.
    IK_Free = 0,

    /// Virtual register interference. There are interfering virtual
    /// registers
    /// assigned to PhysReg or its aliases. This interference could be
    /// resolved
    /// by unassigning those other virtual registers.
    IK_VirtReg,

    /// Register unit interference. A fixed live range is in the way,
    /// typically
    /// argument registers for a call. This can't be resolved by unassigning
    /// other virtual registers.
    IK_RegUnit,

    /// RegMask interference. The live range is crossing an instruction with
    /// a
    /// regmask operand that doesn't preserve PhysReg. This typically means
    /// VirtReg is live across a call, and PhysReg isn't call-preserved.
    IK_RegMask
  };

  /// Check for interference before assigning VirtReg to PhysReg.
  /// If this function returns IK_Free, it is legal to assign(VirtReg,
  /// PhysReg). When there is more than one kind of interference, the
  /// InterferenceKind with the highest enum value is returned.
  InterferenceKind checkInterference(const CgLiveInterval &VirtReg,
                                     MCRegister PhysReg);

  /// Check for interference in the segment [Start, End) that may prevent
  /// assignment to PhysReg. If this function returns true, there is
  /// interference in the segment [Start, End) of some other interval already
  /// assigned to PhysReg. If this function returns false, PhysReg is free at
  /// the segment [Start, End).
  bool checkInterference(CgSlotIndex Start, CgSlotIndex End,
                         MCRegister PhysReg);

  /// Assign VirtReg to PhysReg.
  /// This will mark VirtReg's live range as occupied in the CgLiveRegMatrix
  /// and update CgVirtRegMap. The live range is expected to be available in
  /// PhysReg.
  void assign(const CgLiveInterval &VirtReg, MCRegister PhysReg);

  /// Unassign VirtReg from its PhysReg.
  /// Assuming that VirtReg was previously assigned to a PhysReg, this undoes
  /// the assignment and updates CgVirtRegMap accordingly.
  void unassign(const CgLiveInterval &VirtReg);

  /// Returns true if the given \p PhysReg has any live intervals assigned.
  bool isPhysRegUsed(MCRegister PhysReg) const;

  //===--------------------------------------------------------------------===//
  // Low-level interface.
  //===--------------------------------------------------------------------===//
  //
  // Provide access to the underlying LiveIntervalUnions.
  //

  /// Check for regmask interference only.
  /// Return true if VirtReg crosses a regmask operand that clobbers PhysReg.
  /// If PhysReg is null, check if VirtReg crosses any regmask operands.
  bool checkRegMaskInterference(const CgLiveInterval &VirtReg,
                                MCRegister PhysReg = MCRegister::NoRegister);

  /// Check for regunit interference only.
  /// Return true if VirtReg overlaps a fixed assignment of one of PhysRegs's
  /// register units.
  bool checkRegUnitInterference(const CgLiveInterval &VirtReg,
                                MCRegister PhysReg);

  /// Query a line of the assigned virtual register matrix directly.
  /// Use MCRegUnitIterator to enumerate all regunits in the desired PhysReg.
  /// This returns a reference to an internal Query data structure that is
  /// only valid until the next query() call.
  CgLiveIntervalUnion::Query &query(const CgLiveRange &LR, MCRegister RegUnit);

  /// Directly access the live interval unions per regunit.
  /// This returns an array indexed by the regunit number.
  CgLiveIntervalUnion *getLiveUnions() { return &Matrix[0]; }

  Register getOneVReg(unsigned PhysReg) const;
};

} // namespace COMPILER
