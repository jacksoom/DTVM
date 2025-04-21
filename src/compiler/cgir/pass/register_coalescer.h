//===- RegisterCoalescer.h - Register Coalescing Interface ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the abstract interface for register coalescers,
// allowing them to interact with and query register allocators.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/cg_register_class_info.h"
#include "compiler/cgir/pass/live_range_edit.h"
#include "llvm/CodeGen/Register.h"

using namespace llvm;

namespace llvm {
class TargetRegisterClass;
class TargetRegisterInfo;
} // namespace llvm

namespace COMPILER {

class CgInstruction;

class JoinVals;

class CgRegisterCoalescer : public NonCopyable,
                            private CgLiveRangeEdit::Delegate {
  CgFunction *MF = nullptr;
  CgRegisterInfo *MRI = nullptr;
  const TargetRegisterInfo *TRI = nullptr;
  const TargetInstrInfo *TII = nullptr;
  CgLiveIntervals *LIS = nullptr;
  const CgLoopInfo *Loops = nullptr;
  /*
  AliasAnalysis *AA = nullptr;
  */
  CgRegisterClassInfo RegClassInfo;

  LLVMWorkaround *_llvm_workaround;

  /// Position and VReg of a PHI instruction during coalescing.
  struct PHIValPos {
    CgSlotIndex SI;  ///< Slot where this PHI occurs.
    CgRegister Reg;  ///< VReg the PHI occurs in.
    unsigned SubReg; ///< Qualifying subregister for Reg.
  };

  /// Map from debug instruction number to PHI position during coalescing.
  DenseMap<unsigned, PHIValPos> PHIValToPos;
  /// Index of, for each VReg, which debug instruction numbers and
  /// corresponding PHIs are sensitive to coalescing. Each VReg may have
  /// multiple PHI defs, at different positions.
  DenseMap<CgRegister, SmallVector<unsigned, 2>> RegToPHIIdx;

  /// Debug variable location tracking -- for each VReg, maintain an
  /// ordered-by-slot-index set of DBG_VALUEs, to help quick
  /// identification of whether coalescing may change location validity.
  using DbgValueLoc = std::pair<CgSlotIndex, CgInstruction *>;
  DenseMap<CgRegister, std::vector<DbgValueLoc>> DbgVRegToValues;

  /// VRegs may be repeatedly coalesced, and have many DBG_VALUEs attached.
  /// To avoid repeatedly merging sets of DbgValueLocs, instead record
  /// which vregs have been coalesced, and where to. This map is from
  /// vreg => {set of vregs merged in}.
  DenseMap<CgRegister, SmallVector<Register, 4>> DbgMergedVRegNums;

  /// A LaneMask to remember on which subregister live ranges we need to call
  /// shrinkToUses() later.
  LaneBitmask ShrinkMask;

  /// True if the main range of the currently coalesced intervals should be
  /// checked for smaller live intervals.
  bool ShrinkMainRange = false;

  /// True if the coalescer should aggressively coalesce global copies
  /// in favor of keeping local copies.
  bool JoinGlobalCopies = false;

  /// True if the coalescer should aggressively coalesce fall-thru
  /// blocks exclusively containing copies.
  bool JoinSplitEdges = false;

  /// Copy instructions yet to be coalesced.
  SmallVector<CgInstruction *, 8> WorkList;
  SmallVector<CgInstruction *, 8> LocalWorkList;

  /// Set of instruction pointers that have been erased, and
  /// that may be present in WorkList.
  SmallPtrSet<CgInstruction *, 8> ErasedInstrs;

  /// Dead instructions that are about to be deleted.
  SmallVector<CgInstruction *, 8> DeadDefs;

  /// Virtual registers to be considered for register class inflation.
  SmallVector<CgRegister, 8> InflateRegs;

  /// The collection of live intervals which should have been updated
  /// immediately after rematerialiation but delayed until
  /// lateLiveIntervalUpdate is called.
  DenseSet<CgRegister> ToBeUpdated;

  /// Record how many times the large live interval with many valnos
  /// has been tried to join with other live interval.
  DenseMap<CgRegister, unsigned long> LargeLIVisitCounter;

  /// Recursively eliminate dead defs in DeadDefs.
  void eliminateDeadDefs();

  /// allUsesAvailableAt - Return true if all registers used by OrigMI at
  /// OrigIdx are also available with the same value at UseIdx.
  bool allUsesAvailableAt(const CgInstruction *OrigMI, CgSlotIndex OrigIdx,
                          CgSlotIndex UseIdx);

  /// CgLiveRangeEdit callback for eliminateDeadDefs().
  void LRE_WillEraseInstruction(CgInstruction *MI) override;

  /// Coalesce the LocalWorkList.
  void coalesceLocals();

  /// Join compatible live intervals
  void joinAllIntervals();

  /// Coalesce copies in the specified MBB, putting
  /// copies that cannot yet be coalesced into WorkList.
  void copyCoalesceInMBB(CgBasicBlock *MBB);

  /// Tries to coalesce all copies in CurrList. Returns true if any progress
  /// was made.
  bool copyCoalesceWorkList(MutableArrayRef<CgInstruction *> CurrList);

  /// If one def has many copy like uses, and those copy uses are all
  /// rematerialized, the live interval update needed for those
  /// rematerializations will be delayed and done all at once instead
  /// of being done multiple times. This is to save compile cost because
  /// live interval update is costly.
  void lateLiveIntervalUpdate();

  /// Check if the incoming value defined by a COPY at \p SLRQ in the subrange
  /// has no value defined in the predecessors. If the incoming value is the
  /// same as defined by the copy itself, the value is considered undefined.
  bool copyValueUndefInPredecessors(CgLiveRange &S, const CgBasicBlock *MBB,
                                    CgLiveQueryResult SLRQ);

  /// Set necessary undef flags on subregister uses after pruning out undef
  /// lane segments from the subrange.
  void setUndefOnPrunedSubRegUses(CgLiveInterval &LI, CgRegister Reg,
                                  LaneBitmask PrunedLanes);

  /// Attempt to join intervals corresponding to SrcReg/DstReg, which are the
  /// src/dst of the copy instruction CopyMI.  This returns true if the copy
  /// was successfully coalesced away. If it is not currently possible to
  /// coalesce this interval, but it may be possible if other things get
  /// coalesced, then it returns true by reference in 'Again'.
  bool joinCopy(CgInstruction *CopyMI, bool &Again);

  /// Attempt to join these two intervals.  On failure, this
  /// returns false.  The output "SrcInt" will not have been modified, so we
  /// can use this information below to update aliases.
  bool joinIntervals(CgCoalescerPair &CP);

  /// Attempt joining two virtual registers. Return true on success.
  bool joinVirtRegs(CgCoalescerPair &CP);

  /// If a live interval has many valnos and is coalesced with other
  /// live intervals many times, we regard such live interval as having
  /// high compile time cost.
  bool isHighCostLiveInterval(CgLiveInterval &LI);

  /// Attempt joining with a reserved physreg.
  bool joinReservedPhysReg(CgCoalescerPair &CP);

  /// Add the CgLiveRange @p ToMerge as a subregister liverange of @p LI.
  /// Subranges in @p LI which only partially interfere with the desired
  /// LaneMask are split as necessary. @p LaneMask are the lanes that
  /// @p ToMerge will occupy in the coalescer register. @p LI has its subrange
  /// lanemasks already adjusted to the coalesced register.
  void mergeSubRangeInto(CgLiveInterval &LI, const CgLiveRange &ToMerge,
                         LaneBitmask LaneMask, CgCoalescerPair &CP,
                         unsigned DstIdx);

  /// Join the liveranges of two subregisters. Joins @p RRange into
  /// @p LRange, @p RRange may be invalid afterwards.
  void joinSubRegRanges(CgLiveRange &LRange, CgLiveRange &RRange,
                        LaneBitmask LaneMask, const CgCoalescerPair &CP);

  /// We found a non-trivially-coalescable copy. If the source value number is
  /// defined by a copy from the destination reg see if we can merge these two
  /// destination reg valno# into a single value number, eliminating a copy.
  /// This returns true if an interval was modified.
  bool adjustCopiesBackFrom(const CgCoalescerPair &CP, CgInstruction *CopyMI);

  /// Return true if there are definitions of IntB
  /// other than BValNo val# that can reach uses of AValno val# of IntA.
  bool hasOtherReachingDefs(CgLiveInterval &IntA, CgLiveInterval &IntB,
                            CgVNInfo *AValNo, CgVNInfo *BValNo);

  /// We found a non-trivially-coalescable copy.
  /// If the source value number is defined by a commutable instruction and
  /// its other operand is coalesced to the copy dest register, see if we
  /// can transform the copy into a noop by commuting the definition.
  /// This returns a pair of two flags:
  /// - the first element is true if an interval was modified,
  /// - the second element is true if the destination interval needs
  ///   to be shrunk after deleting the copy.
  std::pair<bool, bool> removeCopyByCommutingDef(const CgCoalescerPair &CP,
                                                 CgInstruction *CopyMI);

  /// We found a copy which can be moved to its less frequent predecessor.
  bool removePartialRedundancy(const CgCoalescerPair &CP,
                               CgInstruction &CopyMI);

  /// If the source of a copy is defined by a
  /// trivial computation, replace the copy by rematerialize the definition.
  bool reMaterializeTrivialDef(const CgCoalescerPair &CP, CgInstruction *CopyMI,
                               bool &IsDefCopy);

  /// Return true if a copy involving a physreg should be joined.
  bool canJoinPhys(const CgCoalescerPair &CP);

  /// Replace all defs and uses of SrcReg to DstReg and update the subregister
  /// number if it is not zero. If DstReg is a physical register and the
  /// existing subregister number of the def / use being updated is not zero,
  /// make sure to set it to the correct physical subregister.
  void updateRegDefsUses(CgRegister SrcReg, CgRegister DstReg, unsigned SubIdx);

  /// If the given machine operand reads only undefined lanes add an undef
  /// flag.
  /// This can happen when undef uses were previously concealed by a copy
  /// which we coalesced. Example:
  ///    %0:sub0<def,read-undef> = ...
  ///    %1 = COPY %0           <-- Coalescing COPY reveals undef
  ///       = use %1:sub1       <-- hidden undef use
  void addUndefFlag(const CgLiveInterval &Int, CgSlotIndex UseIdx,
                    CgOperand &MO, unsigned SubRegIdx);

  /// Handle copies of undef values. If the undef value is an incoming
  /// PHI value, it will convert @p CopyMI to an IMPLICIT_DEF.
  /// Returns nullptr if @p CopyMI was not in any way eliminable. Otherwise,
  /// it returns @p CopyMI (which could be an IMPLICIT_DEF at this point).
  CgInstruction *eliminateUndefCopy(CgInstruction *CopyMI);

  /// Check whether or not we should apply the terminal rule on the
  /// destination (Dst) of \p Copy.
  /// When the terminal rule applies, Copy is not profitable to
  /// coalesce.
  /// Dst is terminal if it has exactly one affinity (Dst, Src) and
  /// at least one interference (Dst, Dst2). If Dst is terminal, the
  /// terminal rule consists in checking that at least one of
  /// interfering node, say Dst2, has an affinity of equal or greater
  /// weight with Src.
  /// In that case, Dst2 and Dst will not be able to be both coalesced
  /// with Src. Since Dst2 exposes more coalescing opportunities than
  /// Dst, we can drop \p Copy.
  bool applyTerminalRule(const CgInstruction &Copy) const;

  /// Wrapper method for \see CgLiveIntervals::shrinkToUses.
  /// This method does the proper fixing of the live-ranges when the afore
  /// mentioned method returns true.
  void shrinkToUses(CgLiveInterval *LI,
                    SmallVectorImpl<CgInstruction *> *Dead = nullptr) {
    if (LIS->shrinkToUses(LI, Dead)) {
      /// Check whether or not \p LI is composed by multiple connected
      /// components and if that is the case, fix that.
      SmallVector<CgLiveInterval *, 8> SplitLIs;
      LIS->splitSeparateComponents(*LI, SplitLIs);
    }
  }

  /// Wrapper Method to do all the necessary work when an Instruction is
  /// deleted.
  /// Optimizations should use this to make sure that deleted instructions
  /// are always accounted for.
  void deleteInstr(CgInstruction *MI) {
    ErasedInstrs.insert(MI);
    LIS->RemoveCgInstructionFromMaps(*MI);
    MI->eraseFromParent();
  }

  /// Walk over function and initialize the DbgVRegToValues map.
  void buildVRegToDbgValueMap(CgFunction &MF);

  /// Test whether, after merging, any DBG_VALUEs would refer to a
  /// different value number than before merging, and whether this can
  /// be resolved. If not, mark the DBG_VALUE as being undef.
  void checkMergingChangesDbgValues(CgCoalescerPair &CP, CgLiveRange &LHS,
                                    JoinVals &LHSVals, CgLiveRange &RHS,
                                    JoinVals &RHSVals);

  void checkMergingChangesDbgValuesImpl(Register Reg, CgLiveRange &OtherRange,
                                        CgLiveRange &RegRange, JoinVals &Vals2);

public:
  // static char ID; ///< Class identification, replacement for typeinfo

  CgRegisterCoalescer(CgFunction &MF) { runOnCgFunction(MF); }
  ~CgRegisterCoalescer() { releaseMemory(); }
#if 0
    void getAnalysisUsage(AnalysisUsage &AU) const override;
#endif
  void releaseMemory();

  /// This is the pass entry point.
  bool runOnCgFunction(CgFunction &MF);

  /// Implement the dump method.
  void print(raw_ostream &O) const;
};

/// A helper class for register coalescers. When deciding if
/// two registers can be coalesced, CoalescerPair can determine if a copy
/// instruction would become an identity copy after coalescing.
class CgCoalescerPair {
  const TargetRegisterInfo &TRI;

  /// The register that will be left after coalescing. It can be a
  /// virtual or physical register.
  Register DstReg;

  /// The virtual register that will be coalesced into dstReg.
  Register SrcReg;

  /// The sub-register index of the old DstReg in the new coalesced
  /// register.
  unsigned DstIdx = 0;

  /// The sub-register index of the old SrcReg in the new coalesced
  /// register.
  unsigned SrcIdx = 0;

  /// True when the original copy was a partial subregister copy.
  bool Partial = false;

  /// True when both regs are virtual and newRC is constrained.
  bool CrossClass = false;

  /// True when DstReg and SrcReg are reversed from the original
  /// copy instruction.
  bool Flipped = false;

  /// The register class of the coalesced register, or NULL if DstReg
  /// is a physreg. This register class may be a super-register of both
  /// SrcReg and DstReg.
  const TargetRegisterClass *NewRC = nullptr;

public:
  CgCoalescerPair(const TargetRegisterInfo &tri) : TRI(tri) {}

  /// Create a CoalescerPair representing a virtreg-to-physreg copy.
  /// No need to call setRegisters().
  CgCoalescerPair(Register VirtReg, MCRegister PhysReg,
                  const TargetRegisterInfo &tri)
      : TRI(tri), DstReg(PhysReg), SrcReg(VirtReg) {}

  /// Set registers to match the copy instruction MI. Return
  /// false if MI is not a coalescable copy instruction.
  bool setRegisters(const CgInstruction *);

  /// Swap SrcReg and DstReg. Return false if swapping is impossible
  /// because DstReg is a physical register, or SubIdx is set.
  bool flip();

  /// Return true if MI is a copy instruction that will become
  /// an identity copy after coalescing.
  bool isCoalescable(const CgInstruction *) const;

  /// Return true if DstReg is a physical register.
  bool isPhys() const { return !NewRC; }

  /// Return true if the original copy instruction did not copy
  /// the full register, but was a subreg operation.
  bool isPartial() const { return Partial; }

  /// Return true if DstReg is virtual and NewRC is a smaller
  /// register class than DstReg's.
  bool isCrossClass() const { return CrossClass; }

  /// Return true when getSrcReg is the register being defined by
  /// the original copy instruction.
  bool isFlipped() const { return Flipped; }

  /// Return the register (virtual or physical) that will remain
  /// after coalescing.
  Register getDstReg() const { return DstReg; }

  /// Return the virtual register that will be coalesced away.
  Register getSrcReg() const { return SrcReg; }

  /// Return the subregister index that DstReg will be coalesced into, or
  /// 0.
  unsigned getDstIdx() const { return DstIdx; }

  /// Return the subregister index that SrcReg will be coalesced into, or
  /// 0.
  unsigned getSrcIdx() const { return SrcIdx; }

  /// Return the register class of the coalesced register.
  const TargetRegisterClass *getNewRC() const { return NewRC; }
};
} // namespace COMPILER