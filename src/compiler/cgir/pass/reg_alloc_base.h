
//===- CgRegAllocBase.h - basic regalloc interface and driver -----*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CgRegAllocBase class, which is the skeleton of a basic
// register allocation algorithm and interface for extending it. It provides the
// building blocks on which to construct other experimental allocators and test
// the validity of two principles:
//
// - If virtual and physical register liveness is modeled using intervals, then
// on-the-fly interference checking is cheap. Furthermore, interferences can be
// lazily cached and reused.
//
// - Register allocation complexity, and generated code performance is
// determined by the effectiveness of live range splitting rather than optimal
// coloring.
//
// Following the first principle, interfering checking revolves around the
// CgLiveIntervalUnion data structure.
//
// To fulfill the second principle, the basic allocator provides a driver for
// incremental splitting. It essentially punts on the problem of register
// coloring, instead driving the assignment of virtual to physical registers by
// the cost of splitting. The basic allocator allows for heuristic reassignment
// of registers, if a more sophisticated allocator chooses to do that.
//
// This framework provides a way to engineer the compile time vs. code
// quality trade-off without relying on a particular theoretical solver.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/cg_register_class_info.h"
#include "llvm/ADT/SmallPtrSet.h"

namespace llvm {
class TargetRegisterInfo;
class TargetRegisterClass;
} // namespace llvm

using namespace llvm;

namespace COMPILER {

class CgLiveInterval;
class CgLiveIntervals;
class CgLiveRegMatrix;
class CgInstruction;
class CgRegisterInfo;
class CgSpiller;
class CgVirtRegMap;

/// CgRegAllocBase provides the register allocation driver and interface that
/// can be extended to add interesting heuristics.
///
/// Register allocators must override the selectOrSplit() method to implement
/// live range splitting. They must also override enqueue/dequeue to provide an
/// assignment order.
class CgRegAllocBase : public NonCopyable {
  virtual void anchor();

protected:
  const TargetRegisterInfo *TRI = nullptr;
  CgRegisterInfo *MRI = nullptr;
  CgVirtRegMap *VRM = nullptr;
  CgLiveIntervals *LIS = nullptr;
  CgLiveRegMatrix *Matrix = nullptr;
  CgRegisterClassInfo RegClassInfo;
  const RegClassFilterFunc ShouldAllocateClass;

  /// Inst which is a def of an original reg and whose defs are already all
  /// dead after remat is saved in DeadRemats. The deletion of such inst is
  /// postponed till all the allocations are done, so its remat expr is
  /// always available for the remat of all the siblings of the original reg.
  SmallPtrSet<CgInstruction *, 32> DeadRemats;

  CgRegAllocBase(const RegClassFilterFunc F = allocateAllRegClasses)
      : ShouldAllocateClass(F) {}

  virtual ~CgRegAllocBase() = default;

  // A RegAlloc pass should call this before allocatePhysRegs.
  void init(CgVirtRegMap &vrm, CgLiveIntervals &lis, CgLiveRegMatrix &mat);

  // The top-level driver. The output is a CgVirtRegMap that us updated with
  // physical register assignments.
  void allocatePhysRegs();

  // Include spiller post optimization and removing dead defs left because of
  // rematerialization.
  virtual void postOptimization();

  // Get a temporary reference to a CgSpiller instance.
  virtual CgSpiller &spiller() = 0;

  /// enqueue - Add VirtReg to the priority queue of unassigned registers.
  virtual void enqueueImpl(const CgLiveInterval *LI) = 0;

  /// enqueue - Add VirtReg to the priority queue of unassigned registers.
  void enqueue(const CgLiveInterval *LI);

  /// dequeue - Return the next unassigned register, or NULL.
  virtual const CgLiveInterval *dequeue() = 0;

  // A RegAlloc pass should override this to provide the allocation
  // heuristics. Each call must guarantee forward progess by returning an
  // available PhysReg or new set of split live virtual registers. It is up to
  // the splitter to converge quickly toward fully spilled live ranges.
  virtual MCRegister selectOrSplit(const CgLiveInterval &VirtReg,
                                   SmallVectorImpl<Register> &splitLVRs) = 0;

  // Use this group name for NamedRegionTimer.
  static const char TimerGroupName[];
  static const char TimerGroupDescription[];

  /// Method called when the allocator is about to remove a CgLiveInterval.
  virtual void aboutToRemoveInterval(const CgLiveInterval &LI) {}

public:
  /// VerifyEnabled - True when -verify-regalloc is given.
  static bool VerifyEnabled;

private:
  void seedLiveRegs();
};

} // end namespace COMPILER
