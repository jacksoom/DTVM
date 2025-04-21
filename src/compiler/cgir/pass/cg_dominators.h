
//==- llvm/CodeGen/MachineDominators.h - Machine Dom Calculation -*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines classes mirroring those in llvm/Analysis/Dominators.h,
// but for target-specific code rather than target-independent IR.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/cg_basic_block.h"
#include "compiler/cgir/cg_instruction.h"
#include "compiler/context.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/GenericDomTree.h"
#include "llvm/Support/GenericDomTreeConstruction.h"
#include <cassert>
#include <memory>

namespace llvm {
class raw_ostream;

template <>
inline void DominatorTreeBase<CgBasicBlock, false>::addRoot(CgBasicBlock *MBB) {
  this->Roots.push_back(MBB);
}

extern template class DomTreeNodeBase<CgBasicBlock>;
extern template class DominatorTreeBase<CgBasicBlock, false>; // DomTree
extern template class DominatorTreeBase<CgBasicBlock, true>;  // PostDomTree

} // namespace llvm

namespace COMPILER {
class CgFunction;

using CgDomTree = DomTreeBase<CgBasicBlock>;
using CgDomTreeNode = DomTreeNodeBase<CgBasicBlock>;

//===-------------------------------------
/// DominatorTree Class - Concrete subclass of DominatorTreeBase that is used to
/// compute a normal dominator tree.
///
class CgDominatorTree : public ContextObject {
  /// Helper structure used to hold all the basic blocks
  /// involved in the split of a critical edge.
#if 0
  struct CriticalEdge {
    CgBasicBlock *FromBB;
    CgBasicBlock *ToBB;
    CgBasicBlock *NewBB;
  };

  /// Pile up all the critical edges to be split.
  /// The splitting of a critical edge is local and thus, it is possible
  /// to apply several of those changes at the same time.
  mutable SmallVector<CriticalEdge, 32> CriticalEdgesToSplit;

  /// Remember all the basic blocks that are inserted during
  /// edge splitting.
  /// Invariant: NewBBs == all the basic blocks contained in the NewBB
  /// field of all the elements of CriticalEdgesToSplit.
  /// I.e., forall elt in CriticalEdgesToSplit, it exists BB in NewBBs
  /// such as BB == elt.NewBB.
  mutable SmallSet<CgBasicBlock *, 32> NewBBs;

  /// Apply all the recorded critical edges to the DT.
  /// This updates the underlying DT information in a way that uses
  /// the fast query path of DT as much as possible.
  ///
  /// \post CriticalEdgesToSplit.empty().
  void applySplitCriticalEdges() const;

#endif

  /// The DominatorTreeBase that is used to compute a normal dominator tree.
  CgDomTree *DT = nullptr;

public:
  //  static char ID; // Pass ID, replacement for typeid

  CgDominatorTree(CgFunction &MF);
  ~CgDominatorTree() { releaseMemory(); }

  CgDomTree &getBase() {
    if (!DT)
      DT = newObject<CgDomTree>();
    // applySplitCriticalEdges();
    return *DT;
  }

  CgBasicBlock *getRoot() const {
    // applySplitCriticalEdges();
    return DT->getRoot();
  }

  CgDomTreeNode *getRootNode() const {
    // applySplitCriticalEdges();
    return DT->getRootNode();
  }

  void runOnCgFunction(CgFunction &F);

  void calculate(CgFunction &F);

  bool dominates(const CgDomTreeNode *A, const CgDomTreeNode *B) const {
    // applySplitCriticalEdges();
    return DT->dominates(A, B);
  }

  void getDescendants(CgBasicBlock *A,
                      SmallVectorImpl<CgBasicBlock *> &Result) {
    // applySplitCriticalEdges();
    DT->getDescendants(A, Result);
  }

  bool dominates(const CgBasicBlock *A, const CgBasicBlock *B) const {
    // applySplitCriticalEdges();
    return DT->dominates(A, B);
  }

  // dominates - Return true if A dominates B. This performs the
  // special checks necessary if A and B are in the same basic block.
  bool dominates(const CgInstruction *A, const CgInstruction *B) const {
    // applySplitCriticalEdges();
    const CgBasicBlock *BBA = A->getParent(), *BBB = B->getParent();
    if (BBA != BBB)
      return DT->dominates(BBA, BBB);

    // Loop through the basic block until we find A or B.
    CgBasicBlock::const_iterator I = BBA->begin();
    for (; &*I != A && &*I != B; ++I)
      /*empty*/;

    return &*I == A;
  }

  bool properlyDominates(const CgDomTreeNode *A, const CgDomTreeNode *B) const {
    // applySplitCriticalEdges();
    return DT->properlyDominates(A, B);
  }

  bool properlyDominates(const CgBasicBlock *A, const CgBasicBlock *B) const {
    // applySplitCriticalEdges();
    return DT->properlyDominates(A, B);
  }

  /// findNearestCommonDominator - Find nearest common dominator basic block
  /// for basic block A and B. If there is no such block then return NULL.
  CgBasicBlock *findNearestCommonDominator(CgBasicBlock *A, CgBasicBlock *B) {
    // applySplitCriticalEdges();
    return DT->findNearestCommonDominator(A, B);
  }

  CgDomTreeNode *operator[](CgBasicBlock *BB) const {
    // applySplitCriticalEdges();
    return DT->getNode(BB);
  }

  /// getNode - return the (Post)DominatorTree node for the specified basic
  /// block.  This is the same as using operator[] on this class.
  ///
  CgDomTreeNode *getNode(CgBasicBlock *BB) const {
    // applySplitCriticalEdges();
    return DT->getNode(BB);
  }

  /// addNewBlock - Add a new node to the dominator tree information.  This
  /// creates a new node as a child of DomBB dominator node,linking it into
  /// the children list of the immediate dominator.
  CgDomTreeNode *addNewBlock(CgBasicBlock *BB, CgBasicBlock *DomBB) {
    // applySplitCriticalEdges();
    return DT->addNewBlock(BB, DomBB);
  }

  /// changeImmediateDominator - This method is used to update the dominator
  /// tree information when a node's immediate dominator changes.
  ///
  void changeImmediateDominator(CgBasicBlock *N, CgBasicBlock *NewIDom) {
    // applySplitCriticalEdges();
    DT->changeImmediateDominator(N, NewIDom);
  }

  void changeImmediateDominator(CgDomTreeNode *N, CgDomTreeNode *NewIDom) {
    // applySplitCriticalEdges();
    DT->changeImmediateDominator(N, NewIDom);
  }

  /// eraseNode - Removes a node from  the dominator tree. Block must not
  /// dominate any other blocks. Removes node from its immediate dominator's
  /// children list. Deletes dominator node associated with basic block BB.
  void eraseNode(CgBasicBlock *BB) {
    // applySplitCriticalEdges();
    DT->eraseNode(BB);
  }

  /// splitBlock - BB is split and now it has one successor. Update dominator
  /// tree to reflect this change.
  void splitBlock(CgBasicBlock *NewBB) {
    // applySplitCriticalEdges();
    DT->splitBlock(NewBB);
  }

  /// isReachableFromEntry - Return true if A is dominated by the entry
  /// block of the function containing it.
  bool isReachableFromEntry(const CgBasicBlock *A) {
    // applySplitCriticalEdges();
    return DT->isReachableFromEntry(A);
  }

  void releaseMemory();

  void print(raw_ostream &OS) const;
  void dump() const;

  /// Record that the critical edge (FromBB, ToBB) has been
  /// split with NewBB.
  /// This is best to use this method instead of directly update the
  /// underlying information, because this helps mitigating the
  /// number of time the DT information is invalidated.
  ///
  /// \note Do not use this method with regular edges.
  ///
  /// \note To benefit from the compile time improvement incurred by this
  /// method, the users of this method have to limit the queries to the DT
  /// interface between two edges splitting. In other words, they have to
  /// pack the splitting of critical edges as much as possible.
#if 0
  void recordSplitCriticalEdge(CgBasicBlock *FromBB,
                              CgBasicBlock *ToBB,
                              CgBasicBlock *NewBB) {
    bool Inserted = NewBBs.insert(NewBB).second;
    (void)Inserted;
    assert(Inserted &&
           "A basic block inserted via edge splitting cannot appear twice");
    // CriticalEdgesToSplit.push_back({FromBB, ToBB, NewBB});
  }
#endif
};

} // end namespace COMPILER

namespace llvm {
//===-------------------------------------
/// DominatorTree GraphTraits specialization so the DominatorTree can be
/// iterable by generic graph iterators.
///

template <class Node, class ChildIterator> struct CgDomTreeGraphTraitsBase {
  using NodeRef = Node *;
  using ChildIteratorType = ChildIterator;

  static NodeRef getEntryNode(NodeRef N) { return N; }
  static ChildIteratorType child_begin(NodeRef N) { return N->begin(); }
  static ChildIteratorType child_end(NodeRef N) { return N->end(); }
};

template <class T> struct GraphTraits;

template <>
struct GraphTraits<CgDomTreeNode *>
    : public CgDomTreeGraphTraitsBase<CgDomTreeNode,
                                      CgDomTreeNode::const_iterator> {};

template <>
struct GraphTraits<const CgDomTreeNode *>
    : public CgDomTreeGraphTraitsBase<const CgDomTreeNode,
                                      CgDomTreeNode::const_iterator> {};

template <>
struct GraphTraits<CgDominatorTree *> : public GraphTraits<CgDomTreeNode *> {
  static NodeRef getEntryNode(CgDominatorTree *DT) { return DT->getRootNode(); }
};

} // namespace llvm
