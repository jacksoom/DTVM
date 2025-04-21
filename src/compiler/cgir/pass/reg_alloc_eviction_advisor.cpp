
//===- CgRegAllocEvictionAdvisor.cpp - eviction advisor
//---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implementation of the default eviction advisor and of the Analysis pass.
//
//===----------------------------------------------------------------------===//

#include "reg_alloc_eviction_advisor.h"
#include "allocation_order.h"
#include "cg_register_class_info.h"
#include "cgir/cg_function.h"
#include "cgir/pass/llvm_utils.h"
#include "common/common_defs.h"
#include "live_reg_matrix.h"
#include "reg_alloc_greedy.h"
#include "virt_reg_map.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;
using namespace COMPILER;

static CgRegAllocEvictionAdvisorAnalysis::AdvisorMode Mode =
    CgRegAllocEvictionAdvisorAnalysis::AdvisorMode::Default;

static bool EnableLocalReassignment = false;

static unsigned EvictInterferenceCutoff = 10;

#define DEBUG_TYPE "regalloc"
#ifdef LLVM_HAVE_TF_AOT_REGALLOCEVICTMODEL
#define LLVM_HAVE_TF_AOT
#endif

namespace {
class DefaultEvictionAdvisorAnalysis final
    : public CgRegAllocEvictionAdvisorAnalysis {
public:
  DefaultEvictionAdvisorAnalysis(bool NotAsRequested)
      : CgRegAllocEvictionAdvisorAnalysis(AdvisorMode::Default),
        NotAsRequested(NotAsRequested) {}

  // support for isa<> and dyn_cast.
  static bool classof(const CgRegAllocEvictionAdvisorAnalysis *R) {
    return R->getAdvisorMode() == AdvisorMode::Default;
  }

private:
  std::unique_ptr<CgRegAllocEvictionAdvisor>
  getAdvisor(const CgFunction &MF, const CgRAGreedy &RA) override {
    return std::make_unique<CgDefaultEvictionAdvisor>(MF, RA);
  }
  const bool NotAsRequested;
};
} // namespace

CgRegAllocEvictionAdvisor::CgRegAllocEvictionAdvisor(const CgFunction &MF,
                                                     const CgRAGreedy &RA)
    : MF(MF), RA(RA), Matrix(RA.getInterferenceMatrix()),
      LIS(RA.getLiveIntervals()), VRM(RA.getVirtRegMap()),
      MRI(&VRM->getRegInfo()), TRI(MF.getSubtarget().getRegisterInfo()),
      RegClassInfo(RA.getRegClassInfo()),
      RegCosts(COMPILER::getTargetRegisterCosts(TRI)),
      EnableLocalReassign(EnableLocalReassignment ||
                          MF.getSubtarget().enableRALocalReassignment(
                              MF.getTarget().getOptLevel())) {}

/// shouldEvict - determine if A should evict the assigned live range B. The
/// eviction policy defined by this function together with the allocation order
/// defined by enqueue() decides which registers ultimately end up being split
/// and spilled.
///
/// Cascade numbers are used to prevent infinite loops if this function is a
/// cyclic relation.
///
/// @param A          The live range to be assigned.
/// @param IsHint     True when A is about to be assigned to its preferred
///                   register.
/// @param B          The live range to be evicted.
/// @param BreaksHint True when B is already assigned to its preferred register.
bool CgDefaultEvictionAdvisor::shouldEvict(const CgLiveInterval &A, bool IsHint,
                                           const CgLiveInterval &B,
                                           bool BreaksHint) const {
  bool CanSplit = RA.getExtraInfo().getStage(B) < RS_Spill;

  // Be fairly aggressive about following hints as long as the evictee can be
  // split.
  if (CanSplit && IsHint && !BreaksHint)
    return true;

  if (A.weight() > B.weight()) {
    LLVM_DEBUG(dbgs() << "should evict: " << B << " w= " << B.weight() << '\n');
    return true;
  }
  return false;
}

/// canEvictHintInterference - return true if the interference for VirtReg
/// on the PhysReg, which is VirtReg's hint, can be evicted in favor of VirtReg.
bool CgDefaultEvictionAdvisor::canEvictHintInterference(
    const CgLiveInterval &VirtReg, MCRegister PhysReg,
    const SmallVirtRegSet &FixedRegisters) const {
  EvictionCost MaxCost;
  MaxCost.setBrokenHints(1);
  return canEvictInterferenceBasedOnCost(VirtReg, PhysReg, true, MaxCost,
                                         FixedRegisters);
}

/// canEvictInterferenceBasedOnCost - Return true if all interferences between
/// VirtReg and PhysReg can be evicted.
///
/// @param VirtReg Live range that is about to be assigned.
/// @param PhysReg Desired register for assignment.
/// @param IsHint  True when PhysReg is VirtReg's preferred register.
/// @param MaxCost Only look for cheaper candidates and update with new cost
///                when returning true.
/// @returns True when interference can be evicted cheaper than MaxCost.
bool CgDefaultEvictionAdvisor::canEvictInterferenceBasedOnCost(
    const CgLiveInterval &VirtReg, MCRegister PhysReg, bool IsHint,
    EvictionCost &MaxCost, const SmallVirtRegSet &FixedRegisters) const {
  // It is only possible to evict virtual register interference.
  if (Matrix->checkInterference(VirtReg, PhysReg) > CgLiveRegMatrix::IK_VirtReg)
    return false;

  bool IsLocal = VirtReg.empty() || LIS->intervalIsInOneMBB(VirtReg);

  // Find VirtReg's cascade number. This will be unassigned if VirtReg was never
  // involved in an eviction before. If a cascade number was assigned, deny
  // evicting anything with the same or a newer cascade number. This prevents
  // infinite eviction loops.
  //
  // This works out so a register without a cascade number is allowed to evict
  // anything, and it can be evicted by anything.
  unsigned Cascade = RA.getExtraInfo().getCascadeOrCurrentNext(VirtReg.reg());

  EvictionCost Cost;
  for (MCRegUnitIterator Units(PhysReg, TRI); Units.isValid(); ++Units) {
    CgLiveIntervalUnion::Query &Q = Matrix->query(VirtReg, *Units);
    // If there is 10 or more interferences, chances are one is heavier.
    const auto &Interferences = Q.interferingVRegs(EvictInterferenceCutoff);
    if (Interferences.size() >= EvictInterferenceCutoff)
      return false;

    // Check if any interfering live range is heavier than MaxWeight.
    for (const CgLiveInterval *Intf : reverse(Interferences)) {
      assert(Register::isVirtualRegister(Intf->reg()) &&
             "Only expecting virtual register interference from query");

      // Do not allow eviction of a virtual register if we are in the middle
      // of last-chance recoloring and this virtual register is one that we
      // have scavenged a physical register for.
      if (FixedRegisters.count(Intf->reg()))
        return false;

      // Never evict spill products. They cannot split or spill.
      if (RA.getExtraInfo().getStage(*Intf) == RS_Done)
        return false;
      // Once a live range becomes small enough, it is urgent that we find a
      // register for it. This is indicated by an infinite spill weight. These
      // urgent live ranges get to evict almost anything.
      //
      // Also allow urgent evictions of unspillable ranges from a strictly
      // larger allocation order.
      bool Urgent =
          !VirtReg.isSpillable() &&
          (Intf->isSpillable() ||
           RegClassInfo.getNumAllocatableRegs(MRI->getRegClass(VirtReg.reg())) <
               RegClassInfo.getNumAllocatableRegs(
                   MRI->getRegClass(Intf->reg())));
      // Only evict older cascades or live ranges without a cascade.
      unsigned IntfCascade = RA.getExtraInfo().getCascade(Intf->reg());
      if (Cascade == IntfCascade)
        return false;

      if (Cascade < IntfCascade) {
        if (!Urgent)
          return false;
        // We permit breaking cascades for urgent evictions. It should be the
        // last resort, though, so make it really expensive.
        Cost.BrokenHints += 10;
      }
      // Would this break a satisfied hint?
      bool BreaksHint = VRM->hasPreferredPhys(Intf->reg());
      // Update eviction cost.
      Cost.BrokenHints += BreaksHint;
      Cost.MaxWeight = std::max(Cost.MaxWeight, Intf->weight());
      // Abort if this would be too expensive.
      if (!(Cost < MaxCost))
        return false;
      if (Urgent)
        continue;
      // Apply the eviction policy for non-urgent evictions.
      if (!shouldEvict(VirtReg, IsHint, *Intf, BreaksHint))
        return false;
      // If !MaxCost.isMax(), then we're just looking for a cheap register.
      // Evicting another local live range in this case could lead to suboptimal
      // coloring.
      if (!MaxCost.isMax() && IsLocal && LIS->intervalIsInOneMBB(*Intf) &&
          (!EnableLocalReassign || !canReassign(*Intf, PhysReg))) {
        return false;
      }
    }
  }
  MaxCost = Cost;
  return true;
}

MCRegister CgDefaultEvictionAdvisor::tryFindEvictionCandidate(
    const CgLiveInterval &VirtReg, const CgAllocationOrder &Order,
    uint8_t CostPerUseLimit, const SmallVirtRegSet &FixedRegisters) const {
  // Keep track of the cheapest interference seen so far.
  EvictionCost BestCost;
  BestCost.setMax();
  MCRegister BestPhys;
  auto MaybeOrderLimit = getOrderLimit(VirtReg, Order, CostPerUseLimit);
  if (!MaybeOrderLimit)
    return MCRegister::NoRegister;
  unsigned OrderLimit = *MaybeOrderLimit;

  // When we are just looking for a reduced cost per use, don't break any
  // hints, and only evict smaller spill weights.
  if (CostPerUseLimit < uint8_t(~0u)) {
    BestCost.BrokenHints = 0;
    BestCost.MaxWeight = VirtReg.weight();
  }

  for (auto I = Order.begin(), E = Order.getOrderLimitEnd(OrderLimit); I != E;
       ++I) {
    MCRegister PhysReg = *I;
    assert(PhysReg);
    if (!canAllocatePhysReg(CostPerUseLimit, PhysReg) ||
        !canEvictInterferenceBasedOnCost(VirtReg, PhysReg, false, BestCost,
                                         FixedRegisters))
      continue;

    // Best so far.
    BestPhys = PhysReg;

    // Stop if the hint can be used.
    if (I.isHint())
      break;
  }
  return BestPhys;
}

namespace COMPILER {
CgRegAllocEvictionAdvisorAnalysis *createReleaseModeAdvisor() {
  return new DefaultEvictionAdvisorAnalysis(true);
}
} // namespace COMPILER
