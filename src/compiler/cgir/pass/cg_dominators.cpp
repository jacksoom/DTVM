//===- MachineDominators.cpp - Machine Dominator Calculation --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements simple dominator construction algorithms for finding
// forward dominators on machine functions.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/cg_dominators.h"
#include "compiler/cgir/cg_function.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace COMPILER;

namespace llvm {
template class DomTreeNodeBase<CgBasicBlock>;
template class DominatorTreeBase<CgBasicBlock, false>; // DomTreeBase
} // namespace llvm

void CgDominatorTree::runOnCgFunction(CgFunction &F) {
  calculate(F);
  return;
}

void CgDominatorTree::calculate(CgFunction &F) {
  // CriticalEdgesToSplit.clear();
  // NewBBs.clear();
  DT = newObject<CgDomTree>();
  DT->recalculate(F);
}

CgDominatorTree::CgDominatorTree(CgFunction &MF)
    : ContextObject(MF.getContext()) {
  runOnCgFunction(MF);
#ifdef ZEN_ENABLE_MULTIPASS_JIT_LOGGING
  llvm::dbgs() << "\n########## DomTree ##########\n\n";
  dump();
#endif
  MF.DomTree = this;
}

void CgDominatorTree::releaseMemory() {
  // CriticalEdgesToSplit.clear();
  deleteObject(DT);
}

void CgDominatorTree::print(raw_ostream &OS) const {
  if (DT)
    DT->print(OS);
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void CgDominatorTree::dump() const { print(dbgs()); }
#endif

#if 0
void CgDominatorTree::applySplitCriticalEdges() const {
  // Bail out early if there is nothing to do.
  if (CriticalEdgesToSplit.empty())
    return;

  // For each element in CriticalEdgesToSplit, remember whether or not element
  // is the new immediate domminator of its successor. The mapping is done by
  // index, i.e., the information for the ith element of CriticalEdgesToSplit is
  // the ith element of IsNewIDom.
  SmallBitVector IsNewIDom(CriticalEdgesToSplit.size(), true);
  size_t Idx = 0;

  // Collect all the dominance properties info, before invalidating
  // the underlying DT.
  for (CriticalEdge &Edge : CriticalEdgesToSplit) {
    // Update dominator information.
    CgBasicBlock *Succ = Edge.ToBB;
    CgDomTreeNode *SuccDTNode = DT->getNode(Succ);

    for (CgBasicBlock *PredBB : Succ->predecessors()) {
      if (PredBB == Edge.NewBB)
        continue;
      // If we are in this situation:
      // FromBB1        FromBB2
      //    +              +
      //   + +            + +
      //  +   +          +   +
      // ...  Split1  Split2 ...
      //           +   +
      //            + +
      //             +
      //            Succ
      // Instead of checking the domiance property with Split2, we check it with
      // FromBB2 since Split2 is still unknown of the underlying DT structure.
      if (NewBBs.count(PredBB)) {
        assert(PredBB->pred_size() == 1 && "A basic block resulting from a "
                                           "critical edge split has more "
                                           "than one predecessor!");
        PredBB = *PredBB->pred_begin();
      }
      if (!DT->dominates(SuccDTNode, DT->getNode(PredBB))) {
        IsNewIDom[Idx] = false;
        break;
      }
    }
    ++Idx;
  }

  // Now, update DT with the collected dominance properties info.
  Idx = 0;
  for (CriticalEdge &Edge : CriticalEdgesToSplit) {
    // We know FromBB dominates NewBB.
    CgDomTreeNode *NewDTNode = DT->addNewBlock(Edge.NewBB, Edge.FromBB);

    // If all the other predecessors of "Succ" are dominated by "Succ" itself
    // then the new block is the new immediate dominator of "Succ". Otherwise,
    // the new block doesn't dominate anything.
    if (IsNewIDom[Idx])
      DT->changeImmediateDominator(DT->getNode(Edge.ToBB), NewDTNode);
    ++Idx;
  }
  NewBBs.clear();
  CriticalEdgesToSplit.clear();
}

#endif
