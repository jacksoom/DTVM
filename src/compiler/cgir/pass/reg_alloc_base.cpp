
//===- CgRegAllocBase.cpp - Register Allocator Base Class
//-------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CgRegAllocBase class which provides common
// functionality for CgLiveIntervalUnion-based register allocators.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/reg_alloc_base.h"
#include "compiler/cgir/cg_instruction.h"
#include "compiler/cgir/pass/cg_register_info.h"
#include "compiler/cgir/pass/cg_spiller.h"
#include "compiler/cgir/pass/live_interval.h"
#include "compiler/cgir/pass/live_intervals.h"
#include "compiler/cgir/pass/live_reg_matrix.h"
#include "compiler/cgir/pass/virt_reg_map.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Timer.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>

using namespace llvm;
using namespace COMPILER;

#define DEBUG_TYPE "regalloc"

const char CgRegAllocBase::TimerGroupName[] = "regalloc";
const char CgRegAllocBase::TimerGroupDescription[] = "Register Allocation";
bool CgRegAllocBase::VerifyEnabled = false;

//===----------------------------------------------------------------------===//
//                         CgRegAllocBase Implementation
//===----------------------------------------------------------------------===//

// Pin the vtable to this file.
void CgRegAllocBase::anchor() {}

void CgRegAllocBase::init(CgVirtRegMap &vrm, CgLiveIntervals &lis,
                          CgLiveRegMatrix &mat) {
  TRI = &vrm.getTargetRegInfo();
  MRI = &vrm.getRegInfo();
  VRM = &vrm;
  LIS = &lis;
  Matrix = &mat;
  MRI->freezeReservedRegs(vrm.getCgFunction());
  RegClassInfo.runOnCgFunction(vrm.getCgFunction());
}

// Visit all the live registers. If they are already assigned to a physical
// register, unify them with the corresponding CgLiveIntervalUnion, otherwise
// push them on the priority queue for later assignment.
void CgRegAllocBase::seedLiveRegs() {
  NamedRegionTimer T("seed", "Seed Live Regs", TimerGroupName,
                     TimerGroupDescription, TimePassesIsEnabled);
  for (unsigned i = 0, e = MRI->getNumVirtRegs(); i != e; ++i) {
    Register Reg = Register::index2VirtReg(i);
    if (MRI->reg_nodbg_empty(Reg))
      continue;
    enqueue(&LIS->getInterval(Reg));
  }
}

// Top-level driver to manage the queue of unassigned VirtRegs and call the
// selectOrSplit implementation.
void CgRegAllocBase::allocatePhysRegs() {
  seedLiveRegs();

  // Continue assigning vregs one at a time to available physical registers.
  while (const CgLiveInterval *VirtReg = dequeue()) {
    assert(!VRM->hasPhys(VirtReg->reg()) && "Register already assigned");

    // Unused registers can appear when the spiller coalesces snippets.
    if (MRI->reg_nodbg_empty(VirtReg->reg())) {
      LLVM_DEBUG(dbgs() << "Dropping unused " << *VirtReg << '\n');
      aboutToRemoveInterval(*VirtReg);
      LIS->removeInterval(VirtReg->reg());
      continue;
    }

    // Invalidate all interference queries, live ranges could have changed.
    Matrix->invalidateVirtRegs();

    // selectOrSplit requests the allocator to return an available physical
    // register if possible and populate a list of new live intervals that
    // result from splitting.
    LLVM_DEBUG(dbgs() << "\nselectOrSplit "
                      << TRI->getRegClassName(MRI->getRegClass(VirtReg->reg()))
                      << ':' << *VirtReg << " w=" << VirtReg->weight() << '\n');

    using VirtRegVec = SmallVector<Register, 4>;

    VirtRegVec SplitVRegs;
    MCRegister AvailablePhysReg = selectOrSplit(*VirtReg, SplitVRegs);

    if (AvailablePhysReg == ~0u) {
      // selectOrSplit failed to find a register!
      // Probably caused by an inline asm.
      CgInstruction *MI = nullptr;
      for (CgRegisterInfo::reg_instr_iterator
               I = MRI->reg_instr_begin(VirtReg->reg()),
               E = MRI->reg_instr_end();
           I != E;) {
        MI = &*(I++);
        if (MI->isInlineAsm())
          break;
      }

      const TargetRegisterClass *RC = MRI->getRegClass(VirtReg->reg());
      ArrayRef<MCPhysReg> AllocOrder = RegClassInfo.getOrder(RC);
      if (AllocOrder.empty())
        report_fatal_error("no registers from class available to allocate");
      else if (MI && MI->isInlineAsm()) {
        ZEN_UNREACHABLE();
        // MI->emitError("inline assembly requires more registers than
        // available");
      } else if (MI) {
        ZEN_UNREACHABLE();
        // LLVMContext &Context =
        //    MI->getParent()->getParent()->getMMI().getModule()->getContext();
        // Context.emitError("ran out of registers during register
        // allocation");
      } else {
        report_fatal_error("ran out of registers during register allocation");
      }

      // Keep going after reporting the error.
      VRM->assignVirt2Phys(VirtReg->reg(), AllocOrder.front());
    } else if (AvailablePhysReg)
      Matrix->assign(*VirtReg, AvailablePhysReg);

    for (Register Reg : SplitVRegs) {
      assert(LIS->hasInterval(Reg));

      CgLiveInterval *SplitVirtReg = &LIS->getInterval(Reg);
      assert(!VRM->hasPhys(SplitVirtReg->reg()) && "Register already assigned");
      if (MRI->reg_nodbg_empty(SplitVirtReg->reg())) {
        assert(SplitVirtReg->empty() && "Non-empty but used interval");
        LLVM_DEBUG(dbgs() << "not queueing unused  " << *SplitVirtReg << '\n');
        aboutToRemoveInterval(*SplitVirtReg);
        LIS->removeInterval(SplitVirtReg->reg());
        continue;
      }
      LLVM_DEBUG(dbgs() << "queuing new interval: " << *SplitVirtReg << "\n");
      assert(Register::isVirtualRegister(SplitVirtReg->reg()) &&
             "expect split value in virtual register");
      enqueue(SplitVirtReg);
    }
  }
}

void CgRegAllocBase::postOptimization() {
  spiller().postOptimization();
  for (auto *DeadInst : DeadRemats) {
    LIS->RemoveCgInstructionFromMaps(*DeadInst);
    DeadInst->eraseFromParent();
  }
  DeadRemats.clear();
}

void CgRegAllocBase::enqueue(const CgLiveInterval *LI) {
  const Register Reg = LI->reg();

  assert(Reg.isVirtual() && "Can only enqueue virtual registers");

  if (VRM->hasPhys(Reg))
    return;

  const TargetRegisterClass &RC = *MRI->getRegClass(Reg);
  if (ShouldAllocateClass(*TRI, RC)) {
    LLVM_DEBUG(dbgs() << "Enqueuing " << printReg(Reg, TRI)
                      << ", weight: " << LI->weight() << '\n');
    enqueueImpl(LI);
  } else {
    LLVM_DEBUG(dbgs() << "Not enqueueing " << printReg(Reg, TRI)
                      << " in skipped register class\n");
  }
}
