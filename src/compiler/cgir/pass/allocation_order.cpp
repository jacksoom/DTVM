
//===-- llvm/CodeGen/CgAllocationOrder.cpp - Allocation Order
//---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements an allocation order for virtual registers.
//
// The preferred allocation order for a virtual register depends on allocation
// hints and target hooks. The CgAllocationOrder class encapsulates all of that.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/allocation_order.h"
#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/pass/cg_register_class_info.h"
#include "compiler/cgir/pass/cg_register_info.h"
#include "compiler/cgir/pass/live_reg_matrix.h"
#include "compiler/cgir/pass/virt_reg_map.h"
#include "compiler/context.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace COMPILER;

#define DEBUG_TYPE "regalloc"

// Compare CgVirtRegMap::getRegAllocPref().
CgAllocationOrder
CgAllocationOrder::create(unsigned VirtReg, const CgVirtRegMap &VRM,
                          const CgRegisterClassInfo &RegClassInfo,
                          const CgLiveRegMatrix *Matrix) {
  CgFunction &MF = VRM.getCgFunction();
  const TargetRegisterInfo *TRI = &VRM.getTargetRegInfo();
  auto Order = RegClassInfo.getOrder(MF.getRegInfo().getRegClass(VirtReg));
  SmallVector<MCPhysReg, 16> Hints;
  bool HardHints = MF.getContext().getLLVMWorkaround().getRegAllocationHints(
      *TRI, VirtReg, Order, Hints, MF, &VRM, Matrix);

  LLVM_DEBUG({
    if (!Hints.empty()) {
      dbgs() << "hints:";
      for (unsigned I = 0, E = Hints.size(); I != E; ++I)
        dbgs() << ' ' << printReg(Hints[I], TRI);
      dbgs() << '\n';
    }
  });
#ifndef NDEBUG
  for (unsigned I = 0, E = Hints.size(); I != E; ++I)
    assert(is_contained(Order, Hints[I]) &&
           "Target hint is outside allocation order.");
#endif
  return CgAllocationOrder(std::move(Hints), Order, HardHints);
}
