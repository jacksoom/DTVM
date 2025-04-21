//===- CgBlockFrequencyInfo.h - MBB Frequency Analysis -----*- C++ -*-===//
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

#pragma once

#include "compiler/cgir/pass/cg_block_frequency_info_impl.h"
#include "compiler/common/common_defs.h"
#include "compiler/context.h"
#include "llvm/ADT/Optional.h"
#include "llvm/Support/BlockFrequency.h"
#include <cstdint>
#include <memory>

using namespace llvm;

namespace COMPILER {
class CgBasicBlock;
class CgBranchProbabilityInfo;
class CgFunction;
class CgLoop;
class CgLoopInfo;
} // namespace COMPILER

namespace llvm {
class raw_ostream;
template <class BlockT> class BlockFrequencyInfoImpl;

namespace bfi_detail {
template <class BlockT> struct TypeMap;
using namespace COMPILER;
template <> struct TypeMap<CgBasicBlock> {
  using BlockT = CgBasicBlock;
  using BlockKeyT = const CgBasicBlock *;
  using FunctionT = CgFunction;
  using BranchProbabilityInfoT = CgBranchProbabilityInfo;
  using LoopT = CgLoop;
  using LoopInfoT = CgLoopInfo;
};

} // end namespace bfi_detail
} // namespace llvm

namespace COMPILER {
/// CgBlockFrequencyInfo pass uses BlockFrequencyInfoImpl implementation
/// to estimate machine basic block frequencies.
class CgBlockFrequencyInfo : public ContextObject {
  using ImplType = BlockFrequencyInfoImpl<CgBasicBlock>;
  ImplType *MBFI = nullptr;

public:
  CgBlockFrequencyInfo(CgFunction &MF);
  ~CgBlockFrequencyInfo() { releaseMemory(); }

  bool runOnCgFunction(CgFunction &F);

  /// calculate - compute block frequency info for the given function.
  void calculate(const CgFunction &F, const CgBranchProbabilityInfo &MBPI,
                 const CgLoopInfo &MLI);

  void releaseMemory();

  /// getblockFreq - Return block frequency. Return 0 if we don't have the
  /// information. Please note that initial frequency is equal to 1024. It
  /// means that we should not rely on the value itself, but only on the
  /// comparison to the other block frequencies. We do this to avoid using of
  /// floating points. For example, to get the frequency of a block relative
  /// to the entry block, divide the integral value returned by this function
  /// (the BlockFrequency::getFrequency() value) by getEntryFreq().
  BlockFrequency getBlockFreq(const CgBasicBlock *MBB) const;

  /// Compute the frequency of the block, relative to the entry block.
  /// This API assumes getEntryFreq() is non-zero.
  float getBlockFreqRelativeToEntryBlock(const CgBasicBlock *MBB) const {
    uint64_t Freq = getEntryFreq();
    ZEN_ASSERT(Freq != 0);
    return getBlockFreq(MBB).getFrequency() * (1.0f / Freq);
  }

  Optional<uint64_t> getBlockProfileCount(const CgBasicBlock *MBB) const;
  Optional<uint64_t> getProfileCountFromFreq(uint64_t Freq) const;

  bool isIrrLoopHeader(const CgBasicBlock *MBB) const;

  /// incrementally calculate block frequencies when we split edges, to avoid
  /// full CFG traversal.
  void onEdgeSplit(const CgBasicBlock &NewPredecessor,
                   const CgBasicBlock &NewSuccessor,
                   const CgBranchProbabilityInfo &MBPI);

  const CgFunction *getFunction() const;
  const CgBranchProbabilityInfo *getMBPI() const;

  /// Pop up a ghostview window with the current block frequency propagation
  /// rendered using dot.
  void view(const Twine &Name, bool isSimple = true) const;

  // Print the block frequency Freq to OS using the current functions entry
  // frequency to convert freq into a relative decimal form.
  raw_ostream &printBlockFreq(raw_ostream &OS, const BlockFrequency Freq) const;

  // Convenience method that attempts to look up the frequency associated with
  // BB and print it to OS.
  raw_ostream &printBlockFreq(raw_ostream &OS, const CgBasicBlock *MBB) const;

  /// Divide a block's BlockFrequency::getFrequency() value by this value to
  /// obtain the entry block - relative frequency of said block.
  uint64_t getEntryFreq() const;
};

} // namespace COMPILER
