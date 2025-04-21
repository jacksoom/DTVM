//===- llvm/CodeGen/CgLoopInfo.h - Natural Loop Calculator -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CgLoopInfo class that is used to identify natural
// loops and determine the loop depth of various nodes of the CFG.  Note that
// natural loops may actually be several loops that share the same header node.
//
// This analysis calculates the nesting structure of loops in a function.  For
// each natural loop identified, this analysis identifies natural loops
// contained entirely within the loop and the basic blocks the make up the loop.
//
// It can calculate on the fly various bits of information, for example:
//
//  * whether there is a preheader for the loop
//  * the number of back edges to the header
//  * whether or not a particular block branches out of the loop
//  * the successor blocks of the loop
//  * the loop depth
//  * the trip count
//  * etc...
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/common/common_defs.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace COMPILER {
class CgLoop;
class CgFunction;
class CgBasicBlock;
class CgInstruction;
} // namespace COMPILER
using namespace COMPILER;

namespace llvm {
// Implementation in LoopInfoImpl.h
extern template class LoopBase<CgBasicBlock, CgLoop>;
// Implementation in LoopInfoImpl.h
extern template class LoopInfoBase<CgBasicBlock, CgLoop>;
} // namespace llvm

namespace COMPILER {

class CgDominatorTree;

class CgLoop : public LoopBase<CgBasicBlock, CgLoop> {
public:
  /// Return the "top" block in the loop, which is the first block in the
  /// linear layout, ignoring any parts of the loop not contiguous with the
  /// part that contains the header.
  CgBasicBlock *getTopBlock();

  /// Return the "bottom" block in the loop, which is the last block in the
  /// linear layout, ignoring any parts of the loop not contiguous with the
  /// part that contains the header.
  CgBasicBlock *getBottomBlock();

  /// Find the block that contains the loop control variable and the
  /// loop test. This will return the latch block if it's one of the exiting
  /// blocks. Otherwise, return the exiting block. Return 'null' when
  /// multiple exiting blocks are present.
  CgBasicBlock *findLoopControlBlock();

  /// Returns true if the instruction is loop invariant.
  /// I.e., all virtual register operands are defined outside of the loop,
  /// physical registers aren't accessed explicitly, and there are no side
  /// effects that aren't captured by the operands or other flags.
  bool isLoopInvariant(CgInstruction &I) const;

  void dump() const;

private:
  friend class LoopInfoBase<CgBasicBlock, CgLoop>;

  explicit CgLoop(CgBasicBlock *MBB) : LoopBase<CgBasicBlock, CgLoop>(MBB) {}

  CgLoop() = default;
};

class CgLoopInfo : public NonCopyable {
  friend class LoopBase<CgBasicBlock, CgLoop>;

  LoopInfoBase<CgBasicBlock, CgLoop> LI;

public:
  CgLoopInfo(CgFunction &MF);
  explicit CgLoopInfo(CgDominatorTree &MDT) { calculate(MDT); }
  ~CgLoopInfo() { releaseMemory(); }

  LoopInfoBase<CgBasicBlock, CgLoop> &getBase() { return LI; }

  /// Find the block that either is the loop preheader, or could
  /// speculatively be used as the preheader. This is e.g. useful to place
  /// loop setup code. Code that cannot be speculated should not be placed
  /// here. SpeculativePreheader is controlling whether it also tries to
  /// find the speculative preheader if the regular preheader is not present.
  /// With FindMultiLoopPreheader = false, nullptr will be returned if the
  /// found preheader is the preheader of multiple loops.
  CgBasicBlock *findLoopPreheader(CgLoop *L, bool SpeculativePreheader = false,
                                  bool FindMultiLoopPreheader = false) const;

  /// The iterator interface to the top-level loops in the current function.
  using iterator = LoopInfoBase<CgBasicBlock, CgLoop>::iterator;
  inline iterator begin() const { return LI.begin(); }
  inline iterator end() const { return LI.end(); }
  bool empty() const { return LI.empty(); }

  /// Return the innermost loop that BB lives in. If a basic block is in no
  /// loop (for example the entry node), null is returned.
  inline CgLoop *getLoopFor(const CgBasicBlock *BB) const {
    return LI.getLoopFor(BB);
  }

  /// Same as getLoopFor.
  inline const CgLoop *operator[](const CgBasicBlock *BB) const {
    return LI.getLoopFor(BB);
  }

  /// Return the loop nesting level of the specified block.
  inline unsigned getLoopDepth(const CgBasicBlock *BB) const {
    return LI.getLoopDepth(BB);
  }

  /// True if the block is a loop header node.
  inline bool isLoopHeader(const CgBasicBlock *BB) const {
    return LI.isLoopHeader(BB);
  }

  /// Calculate the natural loop information.
  bool runOnCgFunction(CgFunction &F);
  void calculate(CgDominatorTree &MDT);

  void releaseMemory() { LI.releaseMemory(); }

  /// This removes the specified top-level loop from this loop info object.
  /// The loop is not deleted, as it will presumably be inserted into another
  /// loop.
  inline CgLoop *removeLoop(iterator I) { return LI.removeLoop(I); }

  /// Change the top-level loop that contains BB to the specified loop. This
  /// should be used by transformations that restructure the loop hierarchy
  /// tree.
  inline void changeLoopFor(CgBasicBlock *BB, CgLoop *L) {
    LI.changeLoopFor(BB, L);
  }

  /// Replace the specified loop in the top-level loops list with the
  /// indicated loop.
  inline void changeTopLevelLoop(CgLoop *OldLoop, CgLoop *NewLoop) {
    LI.changeTopLevelLoop(OldLoop, NewLoop);
  }

  /// This adds the specified loop to the collection of top-level loops.
  inline void addTopLevelLoop(CgLoop *New) { LI.addTopLevelLoop(New); }

  /// This method completely removes BB from all data structures, including
  /// all of the Loop objects it is nested in and our mapping from
  /// CgBasicBlocks to loops.
  void removeBlock(CgBasicBlock *BB) { LI.removeBlock(BB); }
};

} // end namespace COMPILER

namespace llvm {
// Allow clients to walk the list of nested loops...
template <> struct GraphTraits<const CgLoop *> {
  using NodeRef = const CgLoop *;
  using ChildIteratorType = CgLoopInfo::iterator;

  static NodeRef getEntryNode(const CgLoop *L) { return L; }
  static ChildIteratorType child_begin(NodeRef N) { return N->begin(); }
  static ChildIteratorType child_end(NodeRef N) { return N->end(); }
};

template <> struct GraphTraits<CgLoop *> {
  using NodeRef = CgLoop *;
  using ChildIteratorType = CgLoopInfo::iterator;

  static NodeRef getEntryNode(CgLoop *L) { return L; }
  static ChildIteratorType child_begin(NodeRef N) { return N->begin(); }
  static ChildIteratorType child_end(NodeRef N) { return N->end(); }
};

} // end namespace llvm
