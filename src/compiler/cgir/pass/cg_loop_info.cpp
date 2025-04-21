//===- CgLoopInfo.cpp - Natural Loop Calculator ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CgLoopInfo class that is used to identify natural
// loops and determine the loop depth of various nodes of the CFG.  Note that
// the loops identified may actually be several natural loops that share the
// same header node... not just a single natural loop.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/cg_loop_info.h"
#include "compiler/cgir/cg_basic_block.h"
#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/cg_instruction.h"
#include "compiler/cgir/pass/cg_dominators.h"
#include "compiler/cgir/pass/cg_register_info.h"
#include "llvm/Analysis/LoopInfoImpl.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;
using namespace COMPILER;

namespace llvm {
// TODO: why need explicitly instantiate
// Implementation in LoopInfoImpl.h
template class LoopBase<CgBasicBlock, CgLoop>;
// Implementation in LoopInfoImpl.h
template class LoopInfoBase<CgBasicBlock, CgLoop>;
} // namespace llvm

CgLoopInfo::CgLoopInfo(CgFunction &MF) {
  runOnCgFunction(MF);
  MF.Loops = this;
}

bool CgLoopInfo::runOnCgFunction(CgFunction &F) {
  ZEN_ASSERT(F.DomTree != nullptr);
  auto &MDT = *F.DomTree;

  calculate(MDT);
  return false;
}

void CgLoopInfo::calculate(CgDominatorTree &MDT) {
  releaseMemory();
  LI.analyze(MDT.getBase());
}

CgBasicBlock *CgLoop::getTopBlock() {
  CgBasicBlock *TopMBB = getHeader();
  CgFunction::iterator Begin = TopMBB->getParent()->begin();
  if (TopMBB->getIterator() != Begin) {
    CgBasicBlock *PriorMBB = *std::prev(TopMBB->getIterator());
    while (contains(PriorMBB)) {
      TopMBB = PriorMBB;
      if (TopMBB->getIterator() == Begin)
        break;
      PriorMBB = *std::prev(TopMBB->getIterator());
    }
  }
  return TopMBB;
}

CgBasicBlock *CgLoop::getBottomBlock() {
  CgBasicBlock *BotMBB = getHeader();
  CgFunction::iterator End = BotMBB->getParent()->end();
  if (BotMBB->getIterator() != std::prev(End)) {
    CgBasicBlock *NextMBB = *std::next(BotMBB->getIterator());
    while (contains(NextMBB)) {
      BotMBB = NextMBB;
      if (BotMBB == *std::next(BotMBB->getIterator()))
        break;
      NextMBB = *std::next(BotMBB->getIterator());
    }
  }
  return BotMBB;
}

CgBasicBlock *CgLoop::findLoopControlBlock() {
  if (CgBasicBlock *Latch = getLoopLatch()) {
    if (isLoopExiting(Latch))
      return Latch;
    else
      return getExitingBlock();
  }
  return nullptr;
}

CgBasicBlock *CgLoopInfo::findLoopPreheader(CgLoop *L,
                                            bool SpeculativePreheader,
                                            bool FindMultiLoopPreheader) const {
  if (CgBasicBlock *PB = L->getLoopPreheader())
    return PB;

  if (!SpeculativePreheader)
    return nullptr;

  CgBasicBlock *HB = L->getHeader(), *LB = L->getLoopLatch();
  // if (HB->pred_size() != 2 || HB->hasAddressTaken())
  if (HB->pred_size() != 2)
    return nullptr;
  // Find the predecessor of the header that is not the latch block.
  CgBasicBlock *Preheader = nullptr;
  for (CgBasicBlock *P : HB->predecessors()) {
    if (P == LB)
      continue;
    // Sanity.
    if (Preheader)
      return nullptr;
    Preheader = P;
  }

  // Check if the preheader candidate is a successor of any other loop
  // headers. We want to avoid having two loop setups in the same block.
  if (!FindMultiLoopPreheader) {
    for (CgBasicBlock *S : Preheader->successors()) {
      if (S == HB)
        continue;
      CgLoop *T = getLoopFor(S);
      if (T && T->getHeader() == S)
        return nullptr;
    }
  }
  return Preheader;
}

bool CgLoop::isLoopInvariant(CgInstruction &I) const {
  CgFunction *MF = I.getParent()->getParent();
  CgRegisterInfo *MRI = &MF->getRegInfo();
  const TargetSubtargetInfo &ST = MF->getSubtarget();
  const TargetRegisterInfo *TRI = ST.getRegisterInfo();
  const TargetInstrInfo *TII = ST.getInstrInfo();

  // The instruction is loop invariant if all of its operands are.
  for (const CgOperand &MO : I.operands()) {
    if (!MO.isReg())
      continue;

    Register Reg = MO.getReg();
    if (Reg == 0)
      continue;

    // An instruction that uses or defines a physical register can't e.g. be
    // hoisted, so mark this as not invariant.
    if (Register::isPhysicalRegister(Reg)) {
      if (MO.isUse()) {
        // If the physreg has no defs anywhere, it's just an ambient
        // register and we can freely move its uses. Alternatively, if
        // it's allocatable, it could get allocated to something with a
        // def during allocation. However, if the physreg is known to
        // always be caller saved/restored then this use is safe to
        // hoist. if (!MRI->isConstantPhysReg(Reg) &&
        if (!MRI->isConstantPhysReg(Reg) &&
            // TODO !(TRI->isCallerPreservedPhysReg(Reg.asMCReg(),
            // *I.getMF())) &&
            // !TII->isIgnorableUse(MO))
            true)
          return false;
        // Otherwise it's safe to move.
        continue;
      } else if (!MO.isDead()) {
        // A def that isn't dead can't be moved.
        return false;
      } else if (getHeader()->isLiveIn(Reg)) {
        // If the reg is live into the loop, we can't hoist an
        // instruction which would clobber it.
        return false;
      }
    }

    if (!MO.isUse())
      continue;

    assert(!MRI->def_empty(Reg) && "Machine instr not mapped for this vreg?!");

    // If the loop contains the definition of an operand, then the
    // instruction isn't loop invariant.
    for (const CgInstruction &DefInst : MRI->def_instructions(Reg)) {
      if (contains(&DefInst)) {
        return false;
      }
    }
  }

  // If we got this far, the instruction is loop invariant!
  return true;
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void CgLoop::dump() const { print(dbgs()); }
#endif
