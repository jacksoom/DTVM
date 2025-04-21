//===- LiveIntervalCalc.h - Calculate live intervals -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The LiveIntervalCalc class is an extension of LiveRangeCalc targeted to the
// computation and modification of the CgLiveInterval variants of LiveRanges.
// CgLiveIntervals are meant to track liveness of registers and stack slots and
// LiveIntervalCalc adds to LiveRangeCalc all the machinery required to
// construct the liveness of virtual registers tracked by a CgLiveInterval.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/live_range_calc.h"

namespace COMPILER {

class CgLiveIntervalCalc : public CgLiveRangeCalc {
  /// Extend the live range of @p LR to reach all uses of Reg.
  ///
  /// If @p LR is a main range, or if @p LI is null, then all uses must be
  /// jointly dominated by the definitions from @p LR. If @p LR is a subrange
  /// of the live interval @p LI, corresponding to lane mask @p LaneMask,
  /// all uses must be jointly dominated by the definitions from @p LR
  /// together with definitions of other lanes where @p LR becomes undefined
  /// (via <def,read-undef> operands).
  /// If @p LR is a main range, the @p LaneMask should be set to ~0, i.e.
  /// LaneBitmask::getAll().
  void extendToUses(CgLiveRange &LR, Register Reg, LaneBitmask LaneMask,
                    CgLiveInterval *LI = nullptr);

public:
  CgLiveIntervalCalc() = default;

  /// createDeadDefs - Create a dead def in LI for every def operand of Reg.
  /// Each instruction defining Reg gets a new CgVNInfo with a corresponding
  /// minimal live range.
  void createDeadDefs(CgLiveRange &LR, Register Reg);

  /// Extend the live range of @p LR to reach all uses of Reg.
  ///
  /// All uses must be jointly dominated by existing liveness.  PHI-defs are
  /// inserted as needed to preserve SSA form.
  void extendToUses(CgLiveRange &LR, MCRegister PhysReg) {
    extendToUses(LR, PhysReg, LaneBitmask::getAll());
  }

  /// Calculates liveness for the register specified in live interval @p LI.
  /// Creates subregister live ranges as needed if subreg liveness tracking is
  /// enabled.
  void calculate(CgLiveInterval &LI, bool TrackSubRegs);

  /// For live interval \p LI with correct SubRanges construct matching
  /// information for the main live range. Expects the main live range to not
  /// have any segments or value numbers.
  void constructMainRangeFromSubranges(CgLiveInterval &LI);
};

} // namespace COMPILER