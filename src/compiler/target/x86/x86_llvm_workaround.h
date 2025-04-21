// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#pragma once

#include "compiler/cgir/cg_basic_block.h"
#include "compiler/common/llvm_workaround.h"
#include "compiler/llvm-prebuild/Target/X86/X86FrameLowering.h"
#include "compiler/llvm-prebuild/Target/X86/X86InstrInfo.h"
#include "compiler/llvm-prebuild/Target/X86/X86Subtarget.h"

using namespace llvm;

namespace COMPILER {

class X86LLVMWorkaround : public LLVMWorkaround {
public:
  //===----------------------------------------------------------------------===//
  //
  // TargetInstrInfo
  //
  //===----------------------------------------------------------------------===//

  void copyPhysReg(const TargetInstrInfo &TII, CgBasicBlock &MBB,
                   CgBasicBlock::iterator MI, const DebugLoc &DL,
                   MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(const TargetInstrInfo &TII, CgBasicBlock &MBB,
                           CgBasicBlock::iterator MI, Register SrcReg,
                           bool isKill, int FrameIdx,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(const TargetInstrInfo &TII, CgBasicBlock &MBB,
                            CgBasicBlock::iterator MI, Register DestReg,
                            int FrameIdx, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  bool expandPostRAPseudo(const TargetInstrInfo &TII,
                          CgInstruction &MI) const override;

  unsigned isLoadFromStackSlot(const TargetInstrInfo &TII,
                               const CgInstruction &MI,
                               int &FrameIndex) const override;
  unsigned isLoadFromStackSlot(const TargetInstrInfo &TII,
                               const CgInstruction &MI, int &FrameIndex,
                               unsigned &MemBytes) const override;
  void reMaterialize(const TargetInstrInfo &TII, CgBasicBlock &MBB,
                     CgBasicBlock::iterator MI, Register DestReg,
                     unsigned SubIdx, const CgInstruction &Orig,
                     const TargetRegisterInfo &TRI) const override;
  unsigned isStoreToStackSlot(const TargetInstrInfo &TII,
                              const CgInstruction &MI,
                              int &FrameIndex) const override;
  unsigned isStoreToStackSlot(const TargetInstrInfo &TII,
                              const CgInstruction &MI, int &FrameIndex,
                              unsigned &MemBytes) const override;
  //===----------------------------------------------------------------------===//
  //
  // TargetRegisterClass
  //
  //===----------------------------------------------------------------------===//

  ArrayRef<MCPhysReg>
  getRawAllocationOrder(const TargetRegisterClass *TRC,
                        const CgFunction &MF) const override;

  //===----------------------------------------------------------------------===//
  //
  // TargetRegisterInfo
  //
  //===----------------------------------------------------------------------===//

  /// getGPRsForTailCall - Returns a register class with registers that can be
  /// used in forming tail calls.
  const TargetRegisterClass *getGPRsForTailCall(const X86RegisterInfo &TRI,
                                                const CgFunction &MF) const;

  bool hasBasePointer(const X86RegisterInfo *TRI, const CgFunction &MF) const;

  bool canRealignStack(const TargetRegisterInfo &TRI,
                       const CgFunction &MF) const override;

  void eliminateFrameIndex(const TargetRegisterInfo &TRI, CgInstruction &MI,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// findDeadCallerSavedReg - Return a caller-saved register that isn't live
  /// when it reaches the "return" instruction. We can then pop a stack object
  /// to this register without worry about clobbering it.
  unsigned findDeadCallerSavedReg(const X86RegisterInfo *TRI, CgBasicBlock &MBB,
                                  CgBasicBlock::iterator &MBBI) const;

  Register getFrameRegister(const TargetRegisterInfo *TRI,
                            const CgFunction &MF) const override;

  const TargetRegisterClass *
  getLargestLegalSuperClass(const TargetRegisterInfo &TRI,
                            const TargetRegisterClass *RC,
                            const CgFunction &) const override;
  const TargetRegisterClass *
  getPointerRegClass(const TargetRegisterInfo &TRI, const CgFunction &MF,
                     unsigned Kind = 0) const override;
  bool getRegAllocationHints(const TargetRegisterInfo &TRI, Register VirtReg,
                             ArrayRef<MCPhysReg> Order,
                             SmallVectorImpl<MCPhysReg> &Hints,
                             const CgFunction &MF, const CgVirtRegMap *VRM,
                             const CgLiveRegMatrix *Matrix) const override;

  BitVector getReservedRegs(const TargetRegisterInfo &TRI,
                            const CgFunction &MF) const override;
  //===----------------------------------------------------------------------===//
  //
  // TargetFrameLowering
  //
  //===----------------------------------------------------------------------===//

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(const TargetFrameLowering &TFI, CgFunction &MF,
                    CgBasicBlock &MBB) const override;
  void emitEpilogue(const TargetFrameLowering &TFI, CgFunction &MF,
                    CgBasicBlock &MBB) const override;

  void emitSPUpdate(const X86FrameLowering &TFI, CgBasicBlock &MBB,
                    CgBasicBlock::iterator &MBBI, const DebugLoc &DL,
                    int64_t NumBytes, bool InEpilogue) const;
  int mergeSPUpdates(const X86FrameLowering &TFI, CgBasicBlock &MBB,
                     CgBasicBlock::iterator &MBBI,
                     bool doMergeWithPrevious) const;

  void determineCalleeSaves(const TargetFrameLowering *TFI, CgFunction &MF,
                            BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;

  bool
  assignCalleeSavedSpillSlots(const TargetFrameLowering *TFI, CgFunction &MF,
                              const TargetRegisterInfo *TRI,
                              std::vector<CalleeSavedInfo> &CSI) const override;

  bool spillCalleeSavedRegisters(const TargetFrameLowering *TFI,
                                 CgBasicBlock &MBB, CgBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;

  bool
  restoreCalleeSavedRegisters(const TargetFrameLowering *TFI, CgBasicBlock &MBB,
                              CgBasicBlock::iterator MI,
                              MutableArrayRef<CalleeSavedInfo> CSI,
                              const TargetRegisterInfo *TRI) const override;

  bool hasFP(const TargetFrameLowering &TFI,
             const CgFunction &MF) const override;

  bool hasReservedCallFrame(const TargetFrameLowering &TFI,
                            const CgFunction &MF) const override;

  bool needsFrameIndexResolution(const TargetFrameLowering &TFI,
                                 const CgFunction &MF) const override;

  StackOffset getFrameIndexReference(const TargetFrameLowering *TFI,
                                     const CgFunction &MF, int FI,
                                     Register &FrameReg) const override;

  StackOffset getFrameIndexReferenceSP(const X86FrameLowering *TFI,
                                       const CgFunction &MF, int FI,
                                       Register &SPReg, int Adjustment) const;

  CgInstruction *BuildStackAdjustment(const X86FrameLowering &TFI,
                                      CgBasicBlock &MBB,
                                      CgBasicBlock::iterator MBBI,
                                      const DebugLoc &DL, int64_t Offset,
                                      bool InEpilogue) const;

  //===----------------------------------------------------------------------===//
  //
  // LLVMTargetMachine
  //
  //===----------------------------------------------------------------------===//

  X86Subtarget *getSubtargetImpl(const LLVMTargetMachine &TM,
                                 CompileMemPool &MemPool) const override;
};

} // namespace COMPILER
