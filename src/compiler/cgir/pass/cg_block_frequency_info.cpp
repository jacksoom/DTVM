//===- CgBlockFrequencyInfo.cpp - MBB Frequency Analysis -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Loops should be simplified before this analysis.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/cg_block_frequency_info.h"
#include "compiler/cgir/cg_basic_block.h"
#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/pass/cg_branch_probability_info.h"
#include "compiler/cgir/pass/cg_loop_info.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/None.h"
#include "llvm/ADT/iterator.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/GraphWriter.h"
#include <string>

using namespace llvm;
using namespace COMPILER;

#define DEBUG_TYPE "machine-block-freq"

static GVDAGType getGVDT() { return GVDT_Count; }

namespace llvm {

template <> struct GraphTraits<CgBlockFrequencyInfo *> {
  using NodeRef = const CgBasicBlock *;
  using ChildIteratorType = CgBasicBlock::const_succ_iterator;
  // using nodes_iterator = pointer_iterator<CgFunction::const_iterator>;
  using nodes_iterator = CgFunction::const_iterator;

  static NodeRef getEntryNode(const CgBlockFrequencyInfo *G) {
    return &G->getFunction()->front();
  }

  static ChildIteratorType child_begin(const NodeRef N) {
    return N->succ_begin();
  }

  static ChildIteratorType child_end(const NodeRef N) { return N->succ_end(); }

  static nodes_iterator nodes_begin(const CgBlockFrequencyInfo *G) {
    return nodes_iterator(G->getFunction()->begin());
  }

  static nodes_iterator nodes_end(const CgBlockFrequencyInfo *G) {
    return nodes_iterator(G->getFunction()->end());
  }
};

using MBFIDOTGraphTraitsBase =
    BFIDOTGraphTraitsBase<CgBlockFrequencyInfo, CgBranchProbabilityInfo>;

template <>
struct DOTGraphTraits<CgBlockFrequencyInfo *> : public MBFIDOTGraphTraitsBase {
  const CgFunction *CurFunc = nullptr;
  DenseMap<const CgBasicBlock *, int> LayoutOrderMap;

  explicit DOTGraphTraits(bool isSimple = false)
      : MBFIDOTGraphTraitsBase(isSimple) {}

  std::string getNodeLabel(const CgBasicBlock *Node,
                           const CgBlockFrequencyInfo *Graph) {
    int layout_order = -1;
    // Attach additional ordering information if 'isSimple' is false.
    if (!isSimple()) {
      const CgFunction *F = Node->getParent();
      if (!CurFunc || F != CurFunc) {
        if (CurFunc)
          LayoutOrderMap.clear();

        CurFunc = F;
        int O = 0;
        for (auto MBI = F->begin(); MBI != F->end(); ++MBI, ++O) {
          LayoutOrderMap[*MBI] = O;
        }
      }
      layout_order = LayoutOrderMap[Node];
    }
    return MBFIDOTGraphTraitsBase::getNodeLabel(Node, Graph, getGVDT(),
                                                layout_order);
  }

  std::string getNodeAttributes(const CgBasicBlock *Node,
                                const CgBlockFrequencyInfo *Graph) {
    return MBFIDOTGraphTraitsBase::getNodeAttributes(Node, Graph, 10);
  }

  std::string getEdgeAttributes(const CgBasicBlock *Node, EdgeIter EI,
                                const CgBlockFrequencyInfo *MBFI) {
    return MBFIDOTGraphTraitsBase::getEdgeAttributes(Node, EI, MBFI,
                                                     MBFI->getMBPI(), 10);
  }
};

} // end namespace llvm

CgBlockFrequencyInfo::CgBlockFrequencyInfo(CgFunction &MF)
    : ContextObject(MF.getContext()) {
  runOnCgFunction(MF);
  MF.MBFI = this;
}

void CgBlockFrequencyInfo::calculate(const CgFunction &F,
                                     const CgBranchProbabilityInfo &MBPI,
                                     const CgLoopInfo &MLI) {
  if (!MBFI)
    MBFI = newObject<ImplType>();
  MBFI->calculate(F, MBPI, MLI);
#ifdef ZEN_ENABLE_MULTIPASS_JIT_LOGGING

  // view("CgBlockFrequencyDAGS."
  //     + std::to_string(F.getFunction().getFuncIdx()));

  llvm::dbgs()
      << "\n########## Machine Block Frequency Info Dump ##########\n\n";
  MBFI->print(dbgs());

#endif
}

bool CgBlockFrequencyInfo::runOnCgFunction(CgFunction &F) {
  CgBranchProbabilityInfo MBPI;
  ZEN_ASSERT(F.Loops != nullptr);
  CgLoopInfo &MLI = *F.Loops;
  calculate(F, MBPI, MLI);
  return false;
}

void CgBlockFrequencyInfo::releaseMemory() { deleteObject(MBFI); }

/// Pop up a ghostview window with the current block frequency propagation
/// rendered using dot.
void CgBlockFrequencyInfo::view(const Twine &Name, bool isSimple) const {
  // This code is only for debugging.
  ViewGraph(const_cast<CgBlockFrequencyInfo *>(this), Name, isSimple);
}

BlockFrequency
CgBlockFrequencyInfo::getBlockFreq(const CgBasicBlock *MBB) const {
  return MBFI ? MBFI->getBlockFreq(MBB) : 0;
}

Optional<uint64_t>
CgBlockFrequencyInfo::getBlockProfileCount(const CgBasicBlock *MBB) const {
#if 0
    if (!MBFI)
        return None;

    const Function &F = MBFI->getFunction()->getFunction();
    return MBFI->getBlockProfileCount(F, MBB);
#endif
  return None;
}

Optional<uint64_t>
CgBlockFrequencyInfo::getProfileCountFromFreq(uint64_t Freq) const {
#if 0
    if (!MBFI)
        return None;

    const Function &F = MBFI->getFunction()->getFunction();
    return MBFI->getProfileCountFromFreq(F, Freq);
#endif
  return None;
}

bool CgBlockFrequencyInfo::isIrrLoopHeader(const CgBasicBlock *MBB) const {
  assert(MBFI && "Expected analysis to be available");
  return MBFI->isIrrLoopHeader(MBB);
}

void CgBlockFrequencyInfo::onEdgeSplit(const CgBasicBlock &NewPredecessor,
                                       const CgBasicBlock &NewSuccessor,
                                       const CgBranchProbabilityInfo &MBPI) {
  assert(MBFI && "Expected analysis to be available");
  auto NewSuccFreq = MBFI->getBlockFreq(&NewPredecessor) *
                     MBPI.getEdgeProbability(&NewPredecessor, &NewSuccessor);

  MBFI->setBlockFreq(&NewSuccessor, NewSuccFreq.getFrequency());
}

const CgFunction *CgBlockFrequencyInfo::getFunction() const {
  return MBFI ? MBFI->getFunction() : nullptr;
}

const CgBranchProbabilityInfo *CgBlockFrequencyInfo::getMBPI() const {
  return MBFI ? &MBFI->getBPI() : nullptr;
}

raw_ostream &
CgBlockFrequencyInfo::printBlockFreq(raw_ostream &OS,
                                     const BlockFrequency Freq) const {
  return MBFI ? MBFI->printBlockFreq(OS, Freq) : OS;
}

raw_ostream &
CgBlockFrequencyInfo::printBlockFreq(raw_ostream &OS,
                                     const CgBasicBlock *MBB) const {
  return MBFI ? MBFI->printBlockFreq(OS, MBB) : OS;
}

uint64_t CgBlockFrequencyInfo::getEntryFreq() const {
  return MBFI ? MBFI->getEntryFreq() : 0;
}
