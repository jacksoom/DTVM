//===- CgBranchProbabilityInfo.cpp - Cg Branch Probability Info -===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This analysis uses probability info stored in Cg Basic Blocks.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/cg_branch_probability_info.h"
#include "compiler/cgir/cg_basic_block.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace COMPILER;

CgBranchProbabilityInfo::CgBranchProbabilityInfo() {}

void CgBranchProbabilityInfo::anchor() {}

BranchProbability CgBranchProbabilityInfo::getEdgeProbability(
    const CgBasicBlock *Src, CgBasicBlock::const_succ_iterator Dst) const {
  return Src->getSuccProbability(Dst);
}

BranchProbability
CgBranchProbabilityInfo::getEdgeProbability(const CgBasicBlock *Src,
                                            const CgBasicBlock *Dst) const {
  // This is a linear search. Try to use the const_succ_iterator version when
  // possible.
  return getEdgeProbability(Src, find(Src->successors(), Dst));
}

bool CgBranchProbabilityInfo::isEdgeHot(const CgBasicBlock *Src,
                                        const CgBasicBlock *Dst) const {
  BranchProbability HotProb(80, 100);
  return getEdgeProbability(Src, Dst) > HotProb;
}

raw_ostream &CgBranchProbabilityInfo::printEdgeProbability(
    raw_ostream &OS, const CgBasicBlock *Src, const CgBasicBlock *Dst) const {

  const BranchProbability Prob = getEdgeProbability(Src, Dst);
  OS << "edge " << printCgBBReference(*Src) << " -> "
     << printCgBBReference(*Dst) << " probability is " << Prob
     << (isEdgeHot(Src, Dst) ? " [HOT edge]\n" : "\n");

  return OS;
}
