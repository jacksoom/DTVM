
//===- llvm/CodeGen/CgVirtRegMap.cpp - Virtual Register Map
//-----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CgVirtRegMap class.
//
// It also contains implementations of the CgSpiller interface, which, given a
// virtual register map and a machine function, eliminates all virtual
// references by replacing them with physical register references - adding spill
// code as necessary.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/virt_reg_map.h"
#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/pass/live_stacks.h"
#include "compiler/cgir/pass/slot_indexes.h"
#include "compiler/context.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/MC/LaneBitmask.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <iterator>
#include <utility>

using namespace llvm;
using namespace COMPILER;

#define DEBUG_TYPE "regalloc"

//===----------------------------------------------------------------------===//
//  CgVirtRegMap implementation
//===----------------------------------------------------------------------===//

CgVirtRegMap::CgVirtRegMap(CgFunction &MF)
    : Virt2PhysMap(NO_PHYS_REG), Virt2StackSlotMap(NO_STACK_SLOT),
      Virt2SplitMap(0) {
  runOnCgFunction(MF);
  MF.VRM = this;
}

void CgVirtRegMap::runOnCgFunction(CgFunction &mf) {
  MRI = &mf.getRegInfo();
  TII = mf.getSubtarget().getInstrInfo();
  TRI = mf.getSubtarget().getRegisterInfo();
  MF = &mf;
  _llvm_workaround = &(MF->getContext().getLLVMWorkaround());

  Virt2PhysMap.clear();
  Virt2StackSlotMap.clear();
  Virt2SplitMap.clear();
  // Virt2ShapeMap.clear();

  grow();
  return;
}

void CgVirtRegMap::grow() {
  unsigned NumRegs = MF->getRegInfo().getNumVirtRegs();
  Virt2PhysMap.resize(NumRegs);
  Virt2StackSlotMap.resize(NumRegs);
  Virt2SplitMap.resize(NumRegs);
}

void CgVirtRegMap::assignVirt2Phys(Register virtReg, MCPhysReg physReg) {
  assert(virtReg.isVirtual() && Register::isPhysicalRegister(physReg));
  assert(Virt2PhysMap[virtReg.id()] == NO_PHYS_REG &&
         "attempt to assign physical register to already mapped "
         "virtual register");
  assert(!getRegInfo().isReserved(physReg) &&
         "Attempt to map virtReg to a reserved physReg");
  Virt2PhysMap[virtReg.id()] = physReg;
}

unsigned CgVirtRegMap::createSpillSlot(const TargetRegisterClass *RC) {
  unsigned Size = TRI->getSpillSize(*RC);
  Align Alignment = TRI->getSpillAlign(*RC);
  // Set preferred alignment if we are still able to realign the stack
  auto &ST = MF->getSubtarget();
  Align CurrentAlign = ST.getFrameLowering()->getStackAlign();

  // if (Alignment > CurrentAlign &&
  // !ST.getRegisterInfo()->canRealignStack(*MF)) {
  if (Alignment > CurrentAlign &&
      !_llvm_workaround->canRealignStack(*ST.getRegisterInfo(), *MF)) {
    Alignment = CurrentAlign;
  }
  int SS = MF->getFrameInfo().CreateSpillStackObject(Size, Alignment);
  return SS;
}

bool CgVirtRegMap::hasPreferredPhys(Register VirtReg) const {
#if 0
  Register Hint = MRI->getSimpleHint(VirtReg);
  if (!Hint.isValid())
    return false;
  if (Hint.isVirtual())
    Hint = getPhys(Hint);
  return Register(getPhys(VirtReg)) == Hint;
#endif
  return false;
}

bool CgVirtRegMap::hasKnownPreference(Register VirtReg) const {
#if 0
  std::pair<unsigned, unsigned> Hint = MRI->getRegAllocationHint(VirtReg);
  if (Register::isPhysicalRegister(Hint.second))
    return true;
  if (Register::isVirtualRegister(Hint.second))
    return hasPhys(Hint.second);
  return false;
#endif
  return false;
}

int CgVirtRegMap::assignVirt2StackSlot(Register virtReg) {
  assert(virtReg.isVirtual());
  assert(Virt2StackSlotMap[virtReg.id()] == NO_STACK_SLOT &&
         "attempt to assign stack slot to already spilled register");
  const TargetRegisterClass *RC = MF->getRegInfo().getRegClass(virtReg);
  return Virt2StackSlotMap[virtReg.id()] = createSpillSlot(RC);
}

void CgVirtRegMap::assignVirt2StackSlot(Register virtReg, int SS) {
  assert(virtReg.isVirtual());
  assert(Virt2StackSlotMap[virtReg.id()] == NO_STACK_SLOT &&
         "attempt to assign stack slot to already spilled register");
  assert((SS >= 0 || (SS >= MF->getFrameInfo().getObjectIndexBegin())) &&
         "illegal fixed frame index");
  Virt2StackSlotMap[virtReg.id()] = SS;
}

void CgVirtRegMap::print(raw_ostream &OS) const {
  OS << "********** REGISTER MAP **********\n";
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = Register::index2VirtReg(i);
    if (Virt2PhysMap[Reg] != (unsigned)CgVirtRegMap::NO_PHYS_REG) {
      OS << '[' << printReg(Reg, TRI) << " -> "
         << printReg(Virt2PhysMap[Reg], TRI) << "] "
         << TRI->getRegClassName(MRI->getRegClass(Reg)) << "\n";
    }
  }

  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    unsigned Reg = Register::index2VirtReg(i);
    if (Virt2StackSlotMap[Reg] != CgVirtRegMap::NO_STACK_SLOT) {
      OS << '[' << printReg(Reg, TRI) << " -> fi#" << Virt2StackSlotMap[Reg]
         << "] " << TRI->getRegClassName(MRI->getRegClass(Reg)) << "\n";
    }
  }
  OS << '\n';
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void CgVirtRegMap::dump() const { print(dbgs()); }
#endif

void CgVirtRegRewriter::runOnCgFunction(CgFunction &mf) {
  MF = &mf;
  TRI = MF->getSubtarget().getRegisterInfo();
  TII = MF->getSubtarget().getInstrInfo();
  MRI = &MF->getRegInfo();
  Indexes = mf.Indexes;
  LIS = mf.LIS;
  VRM = mf.VRM;
  LLVM_DEBUG(VRM->dump());

  // Add kill flags while we still have virtual registers.
  LIS->addKillFlags(VRM);

  // Live-in lists on basic blocks are required for physregs.
  addMBBLiveIns();

  // Rewrite virtual registers.
  rewrite();

#if 0
  if (DebugVars && ClearVirtRegs) {
    // Write out new DBG_VALUE instructions.

    // We only do this if ClearVirtRegs is specified since this should be the
    // final run of the pass and we don't want to emit them multiple times.
    DebugVars->emitDebugValues(VRM);

    // All machine operands and other references to virtual registers have been
    // replaced. Remove the virtual registers and release all the transient data.
    VRM->clearAllVirt();
    MRI->clearVirtRegs();
  }
#endif

  return;
}

void CgVirtRegRewriter::addLiveInsForSubRanges(const CgLiveInterval &LI,
                                               MCRegister PhysReg) const {
  assert(!LI.empty());
  assert(LI.hasSubRanges());

  using SubRangeIteratorPair = std::pair<const CgLiveInterval::SubRange *,
                                         CgLiveInterval::const_iterator>;

  SmallVector<SubRangeIteratorPair, 4> SubRanges;
  CgSlotIndex First;
  CgSlotIndex Last;
  for (const CgLiveInterval::SubRange &SR : LI.subranges()) {
    SubRanges.push_back(std::make_pair(&SR, SR.begin()));
    if (!First.isValid() || SR.segments.front().start < First)
      First = SR.segments.front().start;
    if (!Last.isValid() || SR.segments.back().end > Last)
      Last = SR.segments.back().end;
  }

  // Check all mbb start positions between First and Last while
  // simultaneously advancing an iterator for each subrange.
  for (CgSlotIndexes::MBBIndexIterator MBBI = Indexes->findMBBIndex(First);
       MBBI != Indexes->MBBIndexEnd() && MBBI->first <= Last; ++MBBI) {
    CgSlotIndex MBBBegin = MBBI->first;
    // Advance all subrange iterators so that their end position is just
    // behind MBBBegin (or the iterator is at the end).
    LaneBitmask LaneMask;
    for (auto &RangeIterPair : SubRanges) {
      const CgLiveInterval::SubRange *SR = RangeIterPair.first;
      CgLiveInterval::const_iterator &SRI = RangeIterPair.second;
      while (SRI != SR->end() && SRI->end <= MBBBegin)
        ++SRI;
      if (SRI == SR->end())
        continue;
      if (SRI->start <= MBBBegin)
        LaneMask |= SR->LaneMask;
    }
    if (LaneMask.none())
      continue;
    CgBasicBlock *MBB = MBBI->second;
    MBB->addLiveIn(PhysReg, LaneMask);
  }
}

// Compute MBB live-in lists from virtual register live ranges and their
// assignments.
void CgVirtRegRewriter::addMBBLiveIns() {
  for (unsigned Idx = 0, IdxE = MRI->getNumVirtRegs(); Idx != IdxE; ++Idx) {
    Register VirtReg = Register::index2VirtReg(Idx);
    if (MRI->reg_nodbg_empty(VirtReg))
      continue;
    CgLiveInterval &LI = LIS->getInterval(VirtReg);
    if (LI.empty() || LIS->intervalIsInOneMBB(LI))
      continue;
    // This is a virtual register that is live across basic blocks. Its
    // assigned PhysReg must be marked as live-in to those blocks.
    Register PhysReg = VRM->getPhys(VirtReg);
    if (PhysReg == CgVirtRegMap::NO_PHYS_REG) {
      // There may be no physical register assigned if only some register
      // classes were already allocated.
      assert(!ClearVirtRegs && "Unmapped virtual register");
      continue;
    }

    if (LI.hasSubRanges()) {
      addLiveInsForSubRanges(LI, PhysReg);
    } else {
      // Go over MBB begin positions and see if we have segments covering
      // them. The following works because segments and the MBBIndex list
      // are both sorted by slot indexes.
      CgSlotIndexes::MBBIndexIterator I = Indexes->MBBIndexBegin();
      for (const auto &Seg : LI) {
        I = Indexes->advanceMBBIndex(I, Seg.start);
        for (; I != Indexes->MBBIndexEnd() && I->first < Seg.end; ++I) {
          CgBasicBlock *MBB = I->second;
          MBB->addLiveIn(PhysReg);
        }
      }
    }
  }

  // Sort and unique MBB LiveIns as we've not checked if SubReg/PhysReg were
  // in each MBB's LiveIns set before calling addLiveIn on them.
  for (CgBasicBlock *MBB : *MF)
    MBB->sortUniqueLiveIns();
}

/// Returns true if the given machine operand \p MO only reads undefined lanes.
/// The function only works for use operands with a subregister set.
bool CgVirtRegRewriter::readsUndefSubreg(const CgOperand &MO) const {
  // Shortcut if the operand is already marked undef.
  if (MO.isUndef())
    return true;

  Register Reg = MO.getReg();
  const CgLiveInterval &LI = LIS->getInterval(Reg);
  const CgInstruction &MI = *MO.getParent();
  CgSlotIndex BaseIndex = LIS->getInstructionIndex(MI);
  // This code is only meant to handle reading undefined subregisters which
  // we couldn't properly detect before.
  assert(LI.liveAt(BaseIndex) &&
         "Reads of completely dead register should be marked undef already");
  unsigned SubRegIdx = MO.getSubReg();
  assert(SubRegIdx != 0 && LI.hasSubRanges());
  LaneBitmask UseMask = TRI->getSubRegIndexLaneMask(SubRegIdx);
  // See if any of the relevant subregister liveranges is defined at this
  // point.
  for (const CgLiveInterval::SubRange &SR : LI.subranges()) {
    if ((SR.LaneMask & UseMask).any() && SR.liveAt(BaseIndex))
      return false;
  }
  return true;
}

void CgVirtRegRewriter::handleIdentityCopy(CgInstruction &MI) {
  if (!MI.isIdentityCopy())
    return;
  LLVM_DEBUG(dbgs() << "Identity copy: " << MI);

  Register DstReg = MI.getOperand(0).getReg();

  // We may have deferred allocation of the virtual register, and the rewrite
  // regs code doesn't handle the liveness update.
  if (DstReg.isVirtual())
    return;

  RewriteRegs.insert(DstReg);

  // Copies like:
  //    %r0 = COPY undef %r0
  //    %al = COPY %al, implicit-def %eax
  // give us additional liveness information: The target (super-)register
  // must not be valid before this point. Replace the COPY with a KILL
  // instruction to maintain this information.
  if (MI.getOperand(1).isUndef() || MI.getNumOperands() > 2) {
    MI.setDesc(TII->get(TargetOpcode::KILL));
    LLVM_DEBUG(dbgs() << "  replace by: " << MI);
    return;
  }

  if (Indexes)
    Indexes->removeSingleCgInstructionFromMaps(MI);
  MI.eraseFromBundle();
  LLVM_DEBUG(dbgs() << "  deleted.\n");
}

/// The liverange splitting logic sometimes produces bundles of copies when
/// subregisters are involved. Expand these into a sequence of copy instructions
/// after processing the last in the bundle. Does not update CgLiveIntervals
/// which we shouldn't need for this instruction anymore.
#if 0
void
CgVirtRegRewriter::expandCopyBundle(CgInstruction &MI) const
{
  if (!MI.isCopy() && !MI.isKill())
    return;

  if (MI.isBundledWithPred() && !MI.isBundledWithSucc()) {
    SmallVector<CgInstruction *, 2> MIs({&MI});

    // Only do this when the complete bundle is made out of COPYs and KILLs.
    CgBasicBlock &MBB = *MI.getParent();
    for (CgBasicBlock::reverse_iterator I =
         std::next(MI.getReverseIterator()), E = MBB.rend();
         I != E && I->isBundledWithSucc(); ++I) {
      if (!I->isCopy() && !I->isKill())
        return;
      MIs.push_back(&*I);
    }
    CgInstruction *FirstMI = MIs.back();

    auto anyRegsAlias = [](const CgInstruction *Dst,
                           ArrayRef<CgInstruction *> Srcs,
                           const TargetRegisterInfo *TRI) {
      for (const CgInstruction *Src : Srcs)
        if (Src != Dst)
          if (TRI->regsOverlap(Dst->getOperand(0).getReg(),
                               Src->getOperand(1).getReg()))
            return true;
      return false;
    };

    // If any of the destination registers in the bundle of copies alias any of
    // the source registers, try to schedule the instructions to avoid any
    // clobbering.
    for (int E = MIs.size(), PrevE = E; E > 1; PrevE = E) {
      for (int I = E; I--; )
        if (!anyRegsAlias(MIs[I], makeArrayRef(MIs).take_front(E), TRI)) {
          if (I + 1 != E)
            std::swap(MIs[I], MIs[E - 1]);
          --E;
        }
      if (PrevE == E) {
		ZEN_UNREACHABLE();
        //MF->getFunction().getContext().emitError(
        //    "register rewriting failed: cycle in copy bundle");
        break;
      }
    }

    CgInstruction *BundleStart = FirstMI;
    for (CgInstruction *BundledMI : llvm::reverse(MIs)) {
      // If instruction is in the middle of the bundle, move it before the
      // bundle starts, otherwise, just unbundle it. When we get to the last
      // instruction, the bundle will have been completely undone.
      if (BundledMI != BundleStart) {
        BundledMI->removeFromBundle();
        MBB.insert(BundleStart, BundledMI);
      } else if (BundledMI->isBundledWithSucc()) {
        BundledMI->unbundleFromSucc();
        BundleStart = &*std::next(BundledMI->getIterator());
      }

      if (Indexes && BundledMI != FirstMI)
        Indexes->insertMachineInstrInMaps(*BundledMI);
    }
  }
}
#endif

/// Check whether (part of) \p SuperPhysReg is live through \p MI.
/// \pre \p MI defines a subregister of a virtual register that
/// has been assigned to \p SuperPhysReg.
bool CgVirtRegRewriter::subRegLiveThrough(const CgInstruction &MI,
                                          MCRegister SuperPhysReg) const {
  CgSlotIndex MIIndex = LIS->getInstructionIndex(MI);
  CgSlotIndex BeforeMIUses = MIIndex.getBaseIndex();
  CgSlotIndex AfterMIDefs = MIIndex.getBoundaryIndex();
  for (MCRegUnitIterator Unit(SuperPhysReg, TRI); Unit.isValid(); ++Unit) {
    const CgLiveRange &CgUnitRange = LIS->getRegUnit(*Unit);
    // If the regunit is live both before and after MI,
    // we assume it is live through.
    // Generally speaking, this is not true, because something like
    // "RU = op RU" would match that description.
    // However, we know that we are trying to assess whether
    // a def of a virtual reg, vreg, is live at the same time of RU.
    // If we are in the "RU = op RU" situation, that means that vreg
    // is defined at the same time as RU (i.e., "vreg, RU = op RU").
    // Thus, vreg and RU interferes and vreg cannot be assigned to
    // SuperPhysReg. Therefore, this situation cannot happen.
    if (CgUnitRange.liveAt(AfterMIDefs) && CgUnitRange.liveAt(BeforeMIUses))
      return true;
  }
  return false;
}

void CgVirtRegRewriter::rewrite() {
  bool NoSubRegLiveness = !MRI->subRegLivenessEnabled();
  SmallVector<Register, 8> SuperDeads;
  SmallVector<Register, 8> SuperDefs;
  SmallVector<Register, 8> SuperKills;

  for (CgFunction::iterator MBBI = MF->begin(), MBBE = MF->end(); MBBI != MBBE;
       ++MBBI) {
    LLVM_DEBUG((*MBBI)->print(dbgs(), Indexes));
    for (CgInstruction &MI : llvm::make_early_inc_range(((*MBBI)->instrs()))) {
      for (CgOperand &MO : MI.operands()) {
        // Make sure MRI knows about registers clobbered by regmasks.
        if (MO.isRegMask())
          MRI->addPhysRegsUsedFromRegMask(MO.getRegMask());

        if (!MO.isReg() || !MO.getReg().isVirtual())
          continue;
        Register VirtReg = MO.getReg();
        MCRegister PhysReg = VRM->getPhys(VirtReg);
        if (PhysReg == CgVirtRegMap::NO_PHYS_REG)
          continue;

        assert(Register(PhysReg).isPhysical());

        RewriteRegs.insert(PhysReg);
        assert(!MRI->isReserved(PhysReg) && "Reserved register assignment");

        // Preserve semantics of sub-register operands.
        unsigned SubReg = MO.getSubReg();
        if (SubReg != 0) {
          if (NoSubRegLiveness || !MRI->shouldTrackSubRegLiveness(VirtReg)) {
            // A virtual register kill refers to the whole register,
            // so we may have to add implicit killed operands for
            // the super-register.  A partial redef always kills and
            // redefines the super-register.
            if ((MO.readsReg() && (MO.isDef() || MO.isKill())) ||
                (MO.isDef() && subRegLiveThrough(MI, PhysReg)))
              SuperKills.push_back(PhysReg);

            if (MO.isDef()) {
              // Also add implicit defs for the super-register.
              if (MO.isDead())
                SuperDeads.push_back(PhysReg);
              else
                SuperDefs.push_back(PhysReg);
            }
          } else {
            if (MO.isUse()) {
              if (readsUndefSubreg(MO))
                // We need to add an <undef> flag if the
                // subregister is completely undefined (and we
                // are not adding super-register defs).
                MO.setIsUndef(true);
            } else if (!MO.isDead()) {
              assert(MO.isDef());
            }
          }

          // The def undef and def internal flags only make sense for
          // sub-register defs, and we are substituting a full
          // physreg.  An implicit killed operand from the SuperKills
          // list will represent the partial read of the
          // super-register.
          if (MO.isDef()) {
            MO.setIsUndef(false);
            MO.setIsInternalRead(false);
          }

          // PhysReg operands cannot have subregister indexes.
          PhysReg = TRI->getSubReg(PhysReg, SubReg);
          assert(PhysReg.isValid() && "Invalid SubReg for physical register");
          MO.setSubReg(0);
        }
        // Rewrite. Note we could have used CgOperand::substPhysReg(),
        // but we need the inlining here.
        MO.setReg(PhysReg);
        // MO.setIsRenamable(true);
      }

      // Add any missing super-register kills after rewriting the whole
      // instruction.
      while (!SuperKills.empty())
        MI.addRegisterKilled(SuperKills.pop_back_val(), TRI, true);

      while (!SuperDeads.empty())
        MI.addRegisterDead(SuperDeads.pop_back_val(), TRI, true);

      while (!SuperDefs.empty())
        MI.addRegisterDefined(SuperDefs.pop_back_val(), TRI);

      LLVM_DEBUG(dbgs() << "> " << MI);

      // expandCopyBundle(MI);

      // We can remove identity copies right now.
      handleIdentityCopy(MI);
    }
  }

  if (LIS) {
    // Don't bother maintaining accurate CgLiveIntervals for registers which
    // were already allocated.
    for (Register PhysReg : RewriteRegs) {
      for (MCRegUnitIterator Units(PhysReg, TRI); Units.isValid(); ++Units) {
        LIS->removeRegUnit(*Units);
      }
    }
  }

  RewriteRegs.clear();
}
