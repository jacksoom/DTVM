//==- RegAllocGreedy.h ------- greedy register allocator  ----------*-C++-*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This file defines the CgRAGreedy function pass for register allocation in
// optimized builds.
//===----------------------------------------------------------------------===//

#pragma once

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/IndexedMap.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <queue>
#include <utility>

#include "compiler/cgir/cg_function.h"
#include "compiler/cgir/pass/calc_spill_weights.h"
#include "compiler/cgir/pass/cg_register_class_info.h"
#include "compiler/cgir/pass/cg_spiller.h"
#include "compiler/cgir/pass/cg_split_kit.h"
#include "compiler/cgir/pass/interference_cache.h"
#include "compiler/cgir/pass/live_interval.h"
#include "compiler/cgir/pass/live_range_edit.h"
#include "compiler/cgir/pass/reg_alloc_base.h"
#include "compiler/cgir/pass/reg_alloc_eviction_advisor.h"
#include "compiler/cgir/pass/spill_placement.h"
#include "compiler/common/llvm_workaround.h"
#include "compiler/context.h"

namespace llvm {
class TargetInstrInfo;
}

namespace COMPILER {
class CgAllocationOrder;
class CgEdgeBundles;
class CgLiveDebugVariables;
class CgLiveIntervals;
class CgLiveRegMatrix;
class CgBasicBlock;
class CgBlockFrequencyInfo;
class CgDominatorTree;
class CgLoop;
class CgLoopInfo;
// class MachineOptimizationRemarkEmitter;
// class MachineOptimizationRemarkMissed;
class CgSlotIndexes;
class CgVirtRegMap;

class LLVM_LIBRARY_VISIBILITY CgRAGreedy : public ContextObject,
                                           public CgRegAllocBase,
                                           private CgLiveRangeEdit::Delegate {
  // Interface to eviction advisers
public:
  /// Track allocation stage and eviction loop prevention during allocation.
  class ExtraRegInfo final {
    // RegInfo - Keep additional information about each live range.
    struct RegInfo {
      LiveRangeStage Stage = RS_New;

      // Cascade - Eviction loop prevention. See
      // canEvictInterferenceBasedOnCost().
      unsigned Cascade = 0;

      RegInfo() = default;
    };

    IndexedMap<RegInfo, VirtReg2IndexFunctor> Info;
    unsigned NextCascade = 1;

  public:
    ExtraRegInfo() = default;
    ExtraRegInfo(const ExtraRegInfo &) = delete;

    LiveRangeStage getStage(Register Reg) const { return Info[Reg].Stage; }

    LiveRangeStage getStage(const CgLiveInterval &VirtReg) const {
      return getStage(VirtReg.reg());
    }

    void setStage(Register Reg, LiveRangeStage Stage) {
      Info.grow(Reg.id());
      Info[Reg].Stage = Stage;
    }

    void setStage(const CgLiveInterval &VirtReg, LiveRangeStage Stage) {
      setStage(VirtReg.reg(), Stage);
    }

    /// Return the current stage of the register, if present, otherwise
    /// initialize it and return that.
    LiveRangeStage getOrInitStage(Register Reg) {
      Info.grow(Reg.id());
      return getStage(Reg);
    }

    unsigned getCascade(Register Reg) const { return Info[Reg].Cascade; }

    void setCascade(Register Reg, unsigned Cascade) {
      Info.grow(Reg.id());
      Info[Reg].Cascade = Cascade;
    }

    unsigned getOrAssignNewCascade(Register Reg) {
      unsigned Cascade = getCascade(Reg);
      if (!Cascade) {
        Cascade = NextCascade++;
        setCascade(Reg, Cascade);
      }
      return Cascade;
    }

    unsigned getCascadeOrCurrentNext(Register Reg) const {
      unsigned Cascade = getCascade(Reg);
      if (!Cascade)
        Cascade = NextCascade;
      return Cascade;
    }

    template <typename Iterator>
    void setStage(Iterator Begin, Iterator End, LiveRangeStage NewStage) {
      for (; Begin != End; ++Begin) {
        Register Reg = *Begin;
        Info.grow(Reg.id());
        if (Info[Reg].Stage == RS_New)
          Info[Reg].Stage = NewStage;
      }
    }
    void LRE_DidCloneVirtReg(Register New, Register Old);
  };

  CgLiveRegMatrix *getInterferenceMatrix() const { return Matrix; }
  CgLiveIntervals *getLiveIntervals() const { return LIS; }
  CgVirtRegMap *getVirtRegMap() const { return VRM; }
  const CgRegisterClassInfo &getRegClassInfo() const { return RegClassInfo; }
  const ExtraRegInfo &getExtraInfo() const { return *ExtraInfo; }
  size_t getQueueSize() const { return Queue.size(); }
  // end (interface to eviction advisers)

private:
  // Convenient shortcuts.
  using PQueue = std::priority_queue<std::pair<unsigned, unsigned>>;
  using SmallLISet = SmallPtrSet<const CgLiveInterval *, 4>;

  // We need to track all tentative recolorings so we can roll back any
  // successful and unsuccessful recoloring attempts.
  using RecoloringStack =
      SmallVector<std::pair<const CgLiveInterval *, MCRegister>, 8>;

  // context
  CgFunction *MF = nullptr;
  LLVMWorkaround *WA = nullptr;

  // Shortcuts to some useful interface.
  const TargetInstrInfo *TII = nullptr;

  // analyses
  CgSlotIndexes *Indexes = nullptr;
  CgBlockFrequencyInfo *MBFI = nullptr;
  CgDominatorTree *DomTree = nullptr;
  CgLoopInfo *Loops = nullptr;
  // MachineOptimizationRemarkEmitter *ORE;
  CgEdgeBundles *Bundles = nullptr;
  CgSpillPlacement *SpillPlacer = nullptr;
  // CgLiveDebugVariables *DebugVars;

  // state
  std::unique_ptr<CgSpiller> SpillerInstance;
  PQueue Queue;
  std::unique_ptr<CgVirtRegAuxInfo> VRAI;
  Optional<ExtraRegInfo> ExtraInfo;
  std::unique_ptr<CgRegAllocEvictionAdvisor> EvictAdvisor;

  // Enum CutOffStage to keep a track whether the register allocation failed
  // because of the cutoffs encountered in last chance recoloring.
  // Note: This is used as bitmask. New value should be next power of 2.
  enum CutOffStage {
    // No cutoffs encountered
    CO_None = 0,

    // lcr-max-depth cutoff encountered
    CO_Depth = 1,

    // lcr-max-interf cutoff encountered
    CO_Interf = 2
  };

  uint8_t CutOffInfo;

#ifndef NDEBUG
  static const char *const StageName[];
#endif

  // splitting state.
  CgSplitAnalysis *SA = nullptr;
  CgSplitEditor *SE = nullptr;

  /// Cached per-block interference maps
  CgInterferenceCache IntfCache;

  /// All basic blocks where the current register has uses.
  SmallVector<CgSpillPlacement::BlockConstraint, 8> SplitConstraints;

  /// Global live range splitting candidate info.
  struct GlobalSplitCandidate {
    // Register intended for assignment, or 0.
    MCRegister PhysReg;

    // SplitKit interval index for this candidate.
    unsigned IntvIdx;

    // Interference for PhysReg.
    CgInterferenceCache::Cursor Intf;

    // Bundles where this candidate should be live.
    BitVector LiveBundles;
    SmallVector<unsigned, 8> ActiveBlocks;

    void reset(CgInterferenceCache &Cache, MCRegister Reg) {
      PhysReg = Reg;
      IntvIdx = 0;
      Intf.setPhysReg(Cache, Reg);
      LiveBundles.clear();
      ActiveBlocks.clear();
    }

    // Set B[I] = C for every live bundle where B[I] was NoCand.
    unsigned getBundles(SmallVectorImpl<unsigned> &B, unsigned C) {
      unsigned Count = 0;
      for (unsigned I : LiveBundles.set_bits())
        if (B[I] == NoCand) {
          B[I] = C;
          Count++;
        }
      return Count;
    }
  };

  /// Candidate info for each PhysReg in CgAllocationOrder.
  /// This vector never shrinks, but grows to the size of the largest register
  /// class.
  SmallVector<GlobalSplitCandidate, 32> GlobalCand;

  enum : unsigned { NoCand = ~0u };

  /// Candidate map. Each edge bundle is assigned to a GlobalCand entry, or to
  /// NoCand which indicates the stack interval.
  SmallVector<unsigned, 32> BundleCand;

  /// Callee-save register cost, calculated once per machine function.
  BlockFrequency CSRCost;

  /// Set of broken hints that may be reconciled later because of eviction.
  SmallSetVector<const CgLiveInterval *, 8> SetOfBrokenHints;

  /// The register cost values. This list will be recreated for each Machine
  /// Function
  ArrayRef<uint8_t> RegCosts;

  /// Flags for the live range priority calculation, determined once per
  /// machine function.
  bool RegClassPriorityTrumpsGlobalness;

  bool ReverseLocalAssignment;

public:
  CgRAGreedy(CgFunction &MF,
             const RegClassFilterFunc F = allocateAllRegClasses);

  void releaseMemory();
  CgSpiller &spiller() override { return *SpillerInstance; }
  void enqueueImpl(const CgLiveInterval *LI) override;
  const CgLiveInterval *dequeue() override;
  MCRegister selectOrSplit(const CgLiveInterval &,
                           SmallVectorImpl<Register> &) override;
  void aboutToRemoveInterval(const CgLiveInterval &) override;

  /// Perform register allocation.
  bool runOnCgFunction(CgFunction &mf);

private:
  MCRegister selectOrSplitImpl(const CgLiveInterval &,
                               SmallVectorImpl<Register> &, SmallVirtRegSet &,
                               RecoloringStack &, unsigned = 0);

  bool LRE_CanEraseVirtReg(Register) override;
  void LRE_WillShrinkVirtReg(Register) override;
  void LRE_DidCloneVirtReg(Register, Register) override;
  void enqueue(PQueue &CurQueue, const CgLiveInterval *LI);
  const CgLiveInterval *dequeue(PQueue &CurQueue);

  bool hasVirtRegAlloc();
  BlockFrequency calcSpillCost();
  bool addSplitConstraints(CgInterferenceCache::Cursor, BlockFrequency &);
  bool addThroughConstraints(CgInterferenceCache::Cursor, ArrayRef<unsigned>);
  bool growRegion(GlobalSplitCandidate &Cand);
  BlockFrequency calcGlobalSplitCost(GlobalSplitCandidate &,
                                     const CgAllocationOrder &Order);
  bool calcCompactRegion(GlobalSplitCandidate &);
  void splitAroundRegion(CgLiveRangeEdit &, ArrayRef<unsigned>);
  void calcGapWeights(MCRegister, SmallVectorImpl<float> &);
  void evictInterference(const CgLiveInterval &, MCRegister,
                         SmallVectorImpl<Register> &);
  bool mayRecolorAllInterferences(MCRegister PhysReg,
                                  const CgLiveInterval &VirtReg,
                                  SmallLISet &RecoloringCandidates,
                                  const SmallVirtRegSet &FixedRegisters);

  MCRegister tryAssign(const CgLiveInterval &, CgAllocationOrder &,
                       SmallVectorImpl<Register> &, const SmallVirtRegSet &);
  MCRegister tryEvict(const CgLiveInterval &, CgAllocationOrder &,
                      SmallVectorImpl<Register> &, uint8_t,
                      const SmallVirtRegSet &);
  MCRegister tryRegionSplit(const CgLiveInterval &, CgAllocationOrder &,
                            SmallVectorImpl<Register> &);
  /// Calculate cost of region splitting.
  unsigned calculateRegionSplitCost(const CgLiveInterval &VirtReg,
                                    CgAllocationOrder &Order,
                                    BlockFrequency &BestCost,
                                    unsigned &NumCands, bool IgnoreCSR);
  /// Perform region splitting.
  unsigned doRegionSplit(const CgLiveInterval &VirtReg, unsigned BestCand,
                         bool HasCompact, SmallVectorImpl<Register> &NewVRegs);
  /// Check other options before using a callee-saved register for the first
  /// time.
  MCRegister tryAssignCSRFirstTime(const CgLiveInterval &VirtReg,
                                   CgAllocationOrder &Order, MCRegister PhysReg,
                                   uint8_t &CostPerUseLimit,
                                   SmallVectorImpl<Register> &NewVRegs);
  void initializeCSRCost();
  unsigned tryBlockSplit(const CgLiveInterval &, CgAllocationOrder &,
                         SmallVectorImpl<Register> &);
  unsigned tryInstructionSplit(const CgLiveInterval &, CgAllocationOrder &,
                               SmallVectorImpl<Register> &);
  unsigned tryLocalSplit(const CgLiveInterval &, CgAllocationOrder &,
                         SmallVectorImpl<Register> &);
  unsigned trySplit(const CgLiveInterval &, CgAllocationOrder &,
                    SmallVectorImpl<Register> &, const SmallVirtRegSet &);
  unsigned tryLastChanceRecoloring(const CgLiveInterval &, CgAllocationOrder &,
                                   SmallVectorImpl<Register> &,
                                   SmallVirtRegSet &, RecoloringStack &,
                                   unsigned);
  bool tryRecoloringCandidates(PQueue &, SmallVectorImpl<Register> &,
                               SmallVirtRegSet &, RecoloringStack &, unsigned);
  void tryHintRecoloring(const CgLiveInterval &);
  void tryHintsRecoloring();

  /// Model the information carried by one end of a copy.
  struct HintInfo {
    /// The frequency of the copy.
    BlockFrequency Freq;
    /// The virtual register or physical register.
    Register Reg;
    /// Its currently assigned register.
    /// In case of a physical register Reg == PhysReg.
    MCRegister PhysReg;

    HintInfo(BlockFrequency Freq, Register Reg, MCRegister PhysReg)
        : Freq(Freq), Reg(Reg), PhysReg(PhysReg) {}
  };
  using HintsInfo = SmallVector<HintInfo, 4>;

  BlockFrequency getBrokenHintFreq(const HintsInfo &, MCRegister);
  void collectHintInfo(Register, HintsInfo &);

  /// Greedy RA statistic to remark.
  struct RAGreedyStats {
    unsigned Reloads = 0;
    unsigned FoldedReloads = 0;
    unsigned ZeroCostFoldedReloads = 0;
    unsigned Spills = 0;
    unsigned FoldedSpills = 0;
    unsigned Copies = 0;
    float ReloadsCost = 0.0f;
    float FoldedReloadsCost = 0.0f;
    float SpillsCost = 0.0f;
    float FoldedSpillsCost = 0.0f;
    float CopiesCost = 0.0f;

    bool isEmpty() {
      return !(Reloads || FoldedReloads || Spills || FoldedSpills ||
               ZeroCostFoldedReloads || Copies);
    }

    void add(RAGreedyStats other) {
      Reloads += other.Reloads;
      FoldedReloads += other.FoldedReloads;
      ZeroCostFoldedReloads += other.ZeroCostFoldedReloads;
      Spills += other.Spills;
      FoldedSpills += other.FoldedSpills;
      Copies += other.Copies;
      ReloadsCost += other.ReloadsCost;
      FoldedReloadsCost += other.FoldedReloadsCost;
      SpillsCost += other.SpillsCost;
      FoldedSpillsCost += other.FoldedSpillsCost;
      CopiesCost += other.CopiesCost;
    }

    // void report(MachineOptimizationRemarkMissed &R);
  };

  /// Compute statistic for a basic block.
  RAGreedyStats computeStats(CgBasicBlock &MBB);

  /// Compute and report statistic through a remark.
  RAGreedyStats reportStats(CgLoop *L);

  /// Report the statistic for each loop.
  void reportStats();
};
} // namespace COMPILER
