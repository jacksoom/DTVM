#pragma once

//===- llvm/CodeGen/CgSpiller.h - CgSpiller -------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

namespace COMPILER {

class CgLiveRangeEdit;
class CgFunction;
class CgVirtRegMap;
class CgVirtRegAuxInfo;

/// CgSpiller interface.
///
/// Implementations are utility classes which insert spill or remat code on
/// demand.
class CgSpiller {
  virtual void anchor();

public:
  virtual ~CgSpiller() = 0;

  /// spill - Spill the LRE.getParent() live interval.
  virtual void spill(CgLiveRangeEdit &LRE) = 0;

  virtual void postOptimization() {}
};

/// Create and return a spiller that will insert spill code directly instead
/// of deferring though CgVirtRegMap.
CgSpiller *cgCreateInlineSpiller(CgFunction &MF, CgVirtRegMap &VRM,
                                 CgVirtRegAuxInfo &VRAI);

} // namespace COMPILER
