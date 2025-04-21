
//===- CgLiveStacks.h - Live Stack Slot Analysis ------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the live stack slot analysis pass. It is analogous to
// live interval analysis except it's analyzing liveness of stack slots rather
// than registers.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/live_interval.h"
#include <cassert>
#include <map>
#include <unordered_map>

namespace llvm {
class raw_ostream;
class TargetRegisterClass;
class TargetRegisterInfo;
} // namespace llvm

using namespace llvm;

namespace COMPILER {

class CgFunction;

class CgLiveStacks : public NonCopyable {
  const TargetRegisterInfo *TRI;

  /// Special pool allocator for VNInfo's (CgLiveInterval val#).
  ///
  CgVNInfo::Allocator VNInfoAllocator;

  /// S2IMap - Stack slot indices to live interval mapping.
  using SS2IntervalMap = std::unordered_map<int, CgLiveInterval>;
  SS2IntervalMap S2IMap;

  /// S2RCMap - Stack slot indices to register class mapping.
  std::map<int, const TargetRegisterClass *> S2RCMap;

public:
  CgLiveStacks(CgFunction &MF) { runOnCgFunction(MF); }
  ~CgLiveStacks() { releaseMemory(); }

  using iterator = SS2IntervalMap::iterator;
  using const_iterator = SS2IntervalMap::const_iterator;

  const_iterator begin() const { return S2IMap.begin(); }
  const_iterator end() const { return S2IMap.end(); }
  iterator begin() { return S2IMap.begin(); }
  iterator end() { return S2IMap.end(); }

  unsigned getNumIntervals() const { return (unsigned)S2IMap.size(); }

  CgLiveInterval &getOrCreateInterval(int Slot, const TargetRegisterClass *RC);

  CgLiveInterval &getInterval(int Slot) {
    assert(Slot >= 0 && "Spill slot indice must be >= 0");
    SS2IntervalMap::iterator I = S2IMap.find(Slot);
    assert(I != S2IMap.end() && "Interval does not exist for stack slot");
    return I->second;
  }

  const CgLiveInterval &getInterval(int Slot) const {
    assert(Slot >= 0 && "Spill slot indice must be >= 0");
    SS2IntervalMap::const_iterator I = S2IMap.find(Slot);
    assert(I != S2IMap.end() && "Interval does not exist for stack slot");
    return I->second;
  }

  bool hasInterval(int Slot) const { return S2IMap.count(Slot); }

  const TargetRegisterClass *getIntervalRegClass(int Slot) const {
    assert(Slot >= 0 && "Spill slot indice must be >= 0");
    std::map<int, const TargetRegisterClass *>::const_iterator I =
        S2RCMap.find(Slot);
    assert(I != S2RCMap.end() &&
           "Register class info does not exist for stack slot");
    return I->second;
  }

  CgVNInfo::Allocator &getVNInfoAllocator() { return VNInfoAllocator; }

  void releaseMemory();

  /// runOnCgFunction - pass entry point
  bool runOnCgFunction(CgFunction &);

  /// print - Implement the dump method.
  void print(raw_ostream &O) const;
};

} // end namespace COMPILER
