//===- CgLiveIntervalUnion.cpp - Live interval union data structure
//---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// CgLiveIntervalUnion represents a coalesced set of live intervals. This may be
// used during coalescing to represent a congruence class, or during register
// allocation to model liveness of a physical register.
//
//===----------------------------------------------------------------------===//

#include "compiler/cgir/pass/live_interval_union.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SparseBitVector.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdlib>

using namespace llvm;

namespace COMPILER {
// Merge a CgLiveInterval's segments. Guarantee no overlaps.
void CgLiveIntervalUnion::unify(const CgLiveInterval &VirtReg,
                                const CgLiveRange &Range) {
  if (Range.empty())
    return;
  ++Tag;

  // Insert each of the virtual register's live segments into the map.
  CgLiveRange::const_iterator RegPos = Range.begin();
  CgLiveRange::const_iterator RegEnd = Range.end();
  SegmentIter SegPos = Segments.find(RegPos->start);

  while (SegPos.valid()) {
    SegPos.insert(RegPos->start, RegPos->end, &VirtReg);
    if (++RegPos == RegEnd)
      return;
    SegPos.advanceTo(RegPos->start);
  }

  // We have reached the end of Segments, so it is no longer necessary to
  // search for the insertion position. It is faster to insert the end first.
  --RegEnd;
  SegPos.insert(RegEnd->start, RegEnd->end, &VirtReg);
  for (; RegPos != RegEnd; ++RegPos, ++SegPos)
    SegPos.insert(RegPos->start, RegPos->end, &VirtReg);
}

// Remove a live virtual register's segments from this union.
void CgLiveIntervalUnion::extract(const CgLiveInterval &VirtReg,
                                  const CgLiveRange &Range) {
  if (Range.empty())
    return;
  ++Tag;

  // Remove each of the virtual register's live segments from the map.
  CgLiveRange::const_iterator RegPos = Range.begin();
  CgLiveRange::const_iterator RegEnd = Range.end();
  SegmentIter SegPos = Segments.find(RegPos->start);

  while (true) {
    assert(SegPos.value() == &VirtReg && "Inconsistent CgLiveInterval");
    SegPos.erase();
    if (!SegPos.valid())
      return;

    // Skip all segments that may have been coalesced.
    RegPos = Range.advanceTo(RegPos, SegPos.start());
    if (RegPos == RegEnd)
      return;

    SegPos.advanceTo(RegPos->start);
  }
}

void CgLiveIntervalUnion::print(raw_ostream &OS,
                                const TargetRegisterInfo *TRI) const {
  if (empty()) {
    OS << " empty\n";
    return;
  }
  for (LiveSegments::const_iterator SI = Segments.begin(); SI.valid(); ++SI) {
    OS << " [" << SI.start() << ' ' << SI.stop()
       << "):" << printReg(SI.value()->reg(), TRI);
  }
  OS << '\n';
}

const CgLiveInterval *CgLiveIntervalUnion::getOneVReg() const {
  if (empty())
    return nullptr;
  for (LiveSegments::const_iterator SI = Segments.begin(); SI.valid(); ++SI) {
    // return the first valid live interval
    return SI.value();
  }
  return nullptr;
}

// Scan the vector of interfering virtual registers in this union. Assume it's
// quite small.
bool CgLiveIntervalUnion::Query::isSeenInterference(
    const CgLiveInterval *VirtReg) const {
  return is_contained(InterferingVRegs, VirtReg);
}

// Collect virtual registers in this union that interfere with this
// query's live virtual register.
//
// The query state is one of:
//
// 1. CheckedFirstInterference == false: Iterators are uninitialized.
// 2. SeenAllInterferences == true: InterferingVRegs complete, iterators unused.
// 3. Iterators left at the last seen intersection.
//
unsigned CgLiveIntervalUnion::Query::collectInterferingVRegs(
    unsigned MaxInterferingRegs) {
  // Fast path return if we already have the desired information.
  if (SeenAllInterferences || InterferingVRegs.size() >= MaxInterferingRegs)
    return InterferingVRegs.size();

  // Set up iterators on the first call.
  if (!CheckedFirstInterference) {
    CheckedFirstInterference = true;

    // Quickly skip interference check for empty sets.
    if (LR->empty() || LiveUnion->empty()) {
      SeenAllInterferences = true;
      return 0;
    }

    // In most cases, the union will start before LR.
    LRI = LR->begin();
    LiveUnionI.setMap(LiveUnion->getMap());
    LiveUnionI.find(LRI->start);
  }

  CgLiveRange::const_iterator LREnd = LR->end();
  const CgLiveInterval *RecentReg = nullptr;
  while (LiveUnionI.valid()) {
    assert(LRI != LREnd && "Reached end of LR");

    // Check for overlapping interference.
    while (LRI->start < LiveUnionI.stop() && LRI->end > LiveUnionI.start()) {
      // This is an overlap, record the interfering register.
      const CgLiveInterval *VReg = LiveUnionI.value();
      if (VReg != RecentReg && !isSeenInterference(VReg)) {
        RecentReg = VReg;
        InterferingVRegs.push_back(VReg);
        if (InterferingVRegs.size() >= MaxInterferingRegs)
          return InterferingVRegs.size();
      }
      // This LiveUnion segment is no longer interesting.
      if (!(++LiveUnionI).valid()) {
        SeenAllInterferences = true;
        return InterferingVRegs.size();
      }
    }

    // The iterators are now not overlapping, LiveUnionI has been advanced
    // beyond LRI.
    assert(LRI->end <= LiveUnionI.start() && "Expected non-overlap");

    // Advance the iterator that ends first.
    LRI = LR->advanceTo(LRI, LiveUnionI.start());
    if (LRI == LREnd)
      break;

    // Detect overlap, handle above.
    if (LRI->start < LiveUnionI.stop())
      continue;

    // Still not overlapping. Catch up LiveUnionI.
    LiveUnionI.advanceTo(LRI->start);
  }
  SeenAllInterferences = true;
  return InterferingVRegs.size();
}

void CgLiveIntervalUnion::Array::init(CgLiveIntervalUnion::Allocator &Alloc,
                                      unsigned NSize) {
  // Reuse existing allocation.
  if (NSize == Size)
    return;
  clear();
  Size = NSize;
  LIUs = static_cast<CgLiveIntervalUnion *>(
      safe_malloc(sizeof(CgLiveIntervalUnion) * NSize));
  for (unsigned i = 0; i != Size; ++i)
    new (LIUs + i) CgLiveIntervalUnion(Alloc);
}

void CgLiveIntervalUnion::Array::clear() {
  if (!LIUs)
    return;
  for (unsigned i = 0; i != Size; ++i)
    LIUs[i].~CgLiveIntervalUnion();
  free(LIUs);
  Size = 0;
  LIUs = nullptr;
}

} // namespace COMPILER