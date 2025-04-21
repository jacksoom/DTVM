//===- LiveIntervalCalc.cpp - Calculate live interval --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implementation of the LiveIntervalCalc class.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/live_interval_calc.h"
#include "compiler/cgir/cg_instruction.h"
#include "compiler/cgir/cg_operand.h"
#include "compiler/cgir/pass/cg_register_info.h"
#include "compiler/cgir/pass/live_interval.h"
#include "compiler/cgir/pass/slot_indexes.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/MC/LaneBitmask.h"
#include "llvm/Support/ErrorHandling.h"
#include <cassert>

using namespace llvm;
using namespace COMPILER;

// Reserve an address that indicates a value that is known to be "undef".
static CgVNInfo UndefVNI(0xbad, CgSlotIndex());

static void createDeadDef(CgSlotIndexes &Indexes, CgVNInfo::Allocator &Alloc,
                          CgLiveRange &LR, const CgOperand &MO) {
  const CgInstruction &MI = *MO.getParent();
  CgSlotIndex DefIdx = Indexes.getInstructionIndex(MI).getRegSlot();

  // Create the def in LR. This may find an existing def.
  LR.createDeadDef(DefIdx, Alloc);
}

void CgLiveIntervalCalc::calculate(CgLiveInterval &LI, bool TrackSubRegs) {
  const CgRegisterInfo *MRI = getRegInfo();
  CgSlotIndexes *Indexes = getIndexes();
  CgVNInfo::Allocator *Alloc = getVNAlloc();

  assert(MRI && Indexes && "call reset() first");

  // Step 1: Create minimal live segments for every definition of Reg.
  // Visit all def operands. If the same instruction has multiple defs of Reg,
  // createDeadDef() will deduplicate.
  const TargetRegisterInfo &TRI = *MRI->getTargetRegisterInfo();
  unsigned Reg = LI.reg();
  for (const CgOperand &MO : MRI->reg_operands(Reg)) {
    if (!MO.isDef() && !MO.readsReg())
      continue;

    unsigned SubReg = MO.getSubReg();
    if (LI.hasSubRanges() || (SubReg != 0 && TrackSubRegs)) {
      LaneBitmask SubMask = SubReg != 0 ? TRI.getSubRegIndexLaneMask(SubReg)
                                        : MRI->getMaxLaneMaskForVReg(Reg);
      // If this is the first time we see a subregister def,
      // initialize subranges by creating a copy of the main range.
      if (!LI.hasSubRanges() && !LI.empty()) {
        LaneBitmask ClassMask = MRI->getMaxLaneMaskForVReg(Reg);
        LI.createSubRangeFrom(*Alloc, ClassMask, LI);
      }

      LI.refineSubRanges(
          *Alloc, SubMask,
          [&MO, Indexes, Alloc](CgLiveInterval::SubRange &SR) {
            if (MO.isDef())
              createDeadDef(*Indexes, *Alloc, SR, MO);
          },
          *Indexes, TRI);
    }

    // Create the def in the main liverange. We do not have to do this
    // if subranges are tracked as we recreate the main range later in
    // this case.
    if (MO.isDef() && !LI.hasSubRanges())
      createDeadDef(*Indexes, *Alloc, LI, MO);
  }

  // We may have created empty live ranges for partially undefined uses, we
  // can't keep them because we won't find defs in them later.
  LI.removeEmptySubRanges();

  const CgFunction *MF = getCgFunction();
  CgDominatorTree *DomTree = getDomTree();
  // Step 2: Extend live segments to all uses, constructing SSA form as
  // necessary.
  if (LI.hasSubRanges()) {
    for (CgLiveInterval::SubRange &S : LI.subranges()) {
      CgLiveIntervalCalc SubLIC;
      SubLIC.reset(MF, Indexes, DomTree, Alloc);
      SubLIC.extendToUses(S, Reg, S.LaneMask, &LI);
    }
    LI.clear();
    constructMainRangeFromSubranges(LI);
  } else {
    resetLiveOutMap();
    extendToUses(LI, Reg, LaneBitmask::getAll());
  }
}

void CgLiveIntervalCalc::constructMainRangeFromSubranges(CgLiveInterval &LI) {
  // First create dead defs at all defs found in subranges.
  CgLiveRange &MainRange = LI;
  assert(MainRange.segments.empty() && MainRange.valnos.empty() &&
         "Expect empty main liverange");

  CgVNInfo::Allocator *Alloc = getVNAlloc();
  for (const CgLiveInterval::SubRange &SR : LI.subranges()) {
    for (const CgVNInfo *VNI : SR.valnos) {
      if (!VNI->isUnused() && !VNI->isPHIDef())
        MainRange.createDeadDef(VNI->def, *Alloc);
    }
  }
  resetLiveOutMap();
  extendToUses(MainRange, LI.reg(), LaneBitmask::getAll(), &LI);
}

void CgLiveIntervalCalc::createDeadDefs(CgLiveRange &LR, Register Reg) {
  const CgRegisterInfo *MRI = getRegInfo();
  CgSlotIndexes *Indexes = getIndexes();
  CgVNInfo::Allocator *Alloc = getVNAlloc();
  assert(MRI && Indexes && "call reset() first");

  // Visit all def operands. If the same instruction has multiple defs of Reg,
  // LR.createDeadDef() will deduplicate.
  for (CgOperand &MO : MRI->def_operands(Reg))
    createDeadDef(*Indexes, *Alloc, LR, MO);
}

void CgLiveIntervalCalc::extendToUses(CgLiveRange &LR, Register Reg,
                                      LaneBitmask Mask, CgLiveInterval *LI) {
  const CgRegisterInfo *MRI = getRegInfo();
  CgSlotIndexes *Indexes = getIndexes();
  SmallVector<CgSlotIndex, 4> Undefs;
  if (LI != nullptr)
    LI->computeSubRangeUndefs(Undefs, Mask, *MRI, *Indexes);

  // Visit all operands that read Reg. This may include partial defs.
  bool IsSubRange = !Mask.all();
  const TargetRegisterInfo &TRI = *MRI->getTargetRegisterInfo();
  for (CgOperand &MO : MRI->reg_operands(Reg)) {
    // Clear all kill flags. They will be reinserted after register
    // allocation by CgLiveIntervals::addKillFlags().
    if (MO.isUse())
      MO.setIsKill(false);
    // MO::readsReg returns "true" for subregister defs. This is for
    // keeping liveness of the entire register (i.e. for the main range
    // of the live interval). For subranges, definitions of
    // non-overlapping subregisters do not count as uses.
    if (!MO.readsReg() || (IsSubRange && MO.isDef()))
      continue;

    unsigned SubReg = MO.getSubReg();
    if (SubReg != 0) {
      LaneBitmask SLM = TRI.getSubRegIndexLaneMask(SubReg);
      if (MO.isDef())
        SLM = ~SLM;
      // Ignore uses not reading the current (sub)range.
      if ((SLM & Mask).none())
        continue;
    }

    // Determine the actual place of the use.
    const CgInstruction *MI = MO.getParent();
    unsigned OpNo = (&MO - &MI->getOperand(0));
    CgSlotIndex UseIdx = Indexes->getInstructionIndex(*MI).getRegSlot();

    // MI is reading Reg. We may have visited MI before if it happens to
    // be reading Reg multiple times. That is OK, extend() is
    // idempotent.
    extend(LR, UseIdx, Reg, Undefs);
  }
}
