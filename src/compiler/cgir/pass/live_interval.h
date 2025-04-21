//===- llvm/CodeGen/CgLiveInterval.h - Interval representation ----*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CgLiveRange and CgLiveInterval classes.  Given some
// numbering of each the machine instructions an interval [i, j) is said to be a
// live range for register v if there is no instruction with number j' >= j
// such that v is live at j' and there is no instruction with number i' < i such
// that v is live at i'. In this implementation ranges can have holes,
// i.e. a range might look like [1,20), [50,65), [1000,1001).  Each
// individual segment is represented as an instance of CgLiveRange::Segment,
// and the whole range is represented as an instance of CgLiveRange.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/cgir/pass/slot_indexes.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/IntEqClasses.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/MC/LaneBitmask.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/MathExtras.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <set>
#include <tuple>
#include <utility>

namespace COMPILER {

class CgCoalescerPair;
class CgLiveIntervals;
class CgRegisterInfo;

/// CgVNInfo - Value Number Information.
/// This class holds information about a machine level values, including
/// definition and use points.
///
class CgVNInfo {
public:
  using Allocator = llvm::BumpPtrAllocator;

  /// The ID number of this value.
  unsigned id;

  /// The index of the defining instruction.
  CgSlotIndex def;

  /// CgVNInfo constructor.
  CgVNInfo(unsigned i, CgSlotIndex d) : id(i), def(d) {}

  /// CgVNInfo constructor, copies values from orig, except for the value
  /// number.
  CgVNInfo(unsigned i, const CgVNInfo &orig) : id(i), def(orig.def) {}

  /// Copy from the parameter into this CgVNInfo.
  void copyFrom(CgVNInfo &src) { def = src.def; }

  /// Returns true if this value is defined by a PHI instruction (or was,
  /// PHI instructions may have been eliminated).
  /// PHI-defs begin at a block boundary, all other defs begin at register or
  /// EC slots.
  bool isPHIDef() const { return def.isBlock(); }

  /// Returns true if this value is unused.
  bool isUnused() const { return !def.isValid(); }

  /// Mark this value as unused.
  void markUnused() { def = CgSlotIndex(); }
};

/// Result of a CgLiveRange query. This class hides the implementation details
/// of live ranges, and it should be used as the primary interface for
/// examining live ranges around instructions.
class CgLiveQueryResult {
  CgVNInfo *const EarlyVal;
  CgVNInfo *const LateVal;
  const CgSlotIndex EndPoint;
  const bool Kill;

public:
  CgLiveQueryResult(CgVNInfo *EarlyVal, CgVNInfo *LateVal, CgSlotIndex EndPoint,
                    bool Kill)
      : EarlyVal(EarlyVal), LateVal(LateVal), EndPoint(EndPoint), Kill(Kill) {}

  /// Return the value that is live-in to the instruction. This is the value
  /// that will be read by the instruction's use operands. Return NULL if no
  /// value is live-in.
  CgVNInfo *valueIn() const { return EarlyVal; }

  /// Return true if the live-in value is killed by this instruction. This
  /// means that either the live range ends at the instruction, or it changes
  /// value.
  bool isKill() const { return Kill; }

  /// Return true if this instruction has a dead def.
  bool isDeadDef() const { return EndPoint.isDead(); }

  /// Return the value leaving the instruction, if any. This can be a
  /// live-through value, or a live def. A dead def returns NULL.
  CgVNInfo *valueOut() const { return isDeadDef() ? nullptr : LateVal; }

  /// Returns the value alive at the end of the instruction, if any. This can
  /// be a live-through value, a live def or a dead def.
  CgVNInfo *valueOutOrDead() const { return LateVal; }

  /// Return the value defined by this instruction, if any. This includes
  /// dead defs, it is the value created by the instruction's def operands.
  CgVNInfo *valueDefined() const {
    return EarlyVal == LateVal ? nullptr : LateVal;
  }

  /// Return the end point of the last live range segment to interact with
  /// the instruction, if any.
  ///
  /// The end point is an invalid CgSlotIndex only if the live range doesn't
  /// intersect the instruction at all.
  ///
  /// The end point may be at or past the end of the instruction's basic
  /// block. That means the value was live out of the block.
  CgSlotIndex endPoint() const { return EndPoint; }
};

/// This class represents the liveness of a register, stack slot, etc.
/// It manages an ordered list of Segment objects.
/// The Segments are organized in a static single assignment form: At places
/// where a new value is defined or different values reach a CFG join a new
/// segment with a new value number is used.
class CgLiveRange {
public:
  /// This represents a simple continuous liveness interval for a value.
  /// The start point is inclusive, the end point exclusive. These intervals
  /// are rendered as [start,end).
  struct Segment {
    CgSlotIndex start;         // Start point of the interval (inclusive)
    CgSlotIndex end;           // End point of the interval (exclusive)
    CgVNInfo *valno = nullptr; // identifier for the value contained in this
                               // segment.

    Segment() = default;

    Segment(CgSlotIndex S, CgSlotIndex E, CgVNInfo *V)
        : start(S), end(E), valno(V) {
      assert(S < E && "Cannot create empty or backwards segment");
    }

    /// Return true if the index is covered by this segment.
    bool contains(CgSlotIndex I) const { return start <= I && I < end; }

    /// Return true if the given interval, [S, E), is covered by this
    /// segment.
    bool containsInterval(CgSlotIndex S, CgSlotIndex E) const {
      assert((S < E) && "Backwards interval?");
      return (start <= S && S < end) && (start < E && E <= end);
    }

    bool operator<(const Segment &Other) const {
      return std::tie(start, end) < std::tie(Other.start, Other.end);
    }
    bool operator==(const Segment &Other) const {
      return start == Other.start && end == Other.end;
    }

    bool operator!=(const Segment &Other) const { return !(*this == Other); }

    void dump() const;
  };

  using Segments = SmallVector<Segment, 2>;
  using VNInfoList = SmallVector<CgVNInfo *, 2>;

  Segments segments; // the liveness segments
  VNInfoList valnos; // value#'s

  // The segment set is used temporarily to accelerate initial computation
  // of live ranges of physical registers in computeRegUnitRange.
  // After that the set is flushed to the segment vector and deleted.
  using SegmentSet = std::set<Segment>;
  std::unique_ptr<SegmentSet> segmentSet;

  using iterator = Segments::iterator;
  using const_iterator = Segments::const_iterator;

  iterator begin() { return segments.begin(); }
  iterator end() { return segments.end(); }

  const_iterator begin() const { return segments.begin(); }
  const_iterator end() const { return segments.end(); }

  using vni_iterator = VNInfoList::iterator;
  using const_vni_iterator = VNInfoList::const_iterator;

  vni_iterator vni_begin() { return valnos.begin(); }
  vni_iterator vni_end() { return valnos.end(); }

  const_vni_iterator vni_begin() const { return valnos.begin(); }
  const_vni_iterator vni_end() const { return valnos.end(); }

  iterator_range<vni_iterator> vnis() {
    return make_range(vni_begin(), vni_end());
  }

  iterator_range<const_vni_iterator> vnis() const {
    return make_range(vni_begin(), vni_end());
  }

  /// Constructs a new CgLiveRange object.
  CgLiveRange(bool UseSegmentSet = false)
      : segmentSet(UseSegmentSet ? std::make_unique<SegmentSet>() : nullptr) {}

  /// Constructs a new CgLiveRange object by copying segments and valnos from
  /// another CgLiveRange.
  CgLiveRange(const CgLiveRange &Other, BumpPtrAllocator &Allocator) {
    assert(Other.segmentSet == nullptr &&
           "Copying of LiveRanges with active SegmentSets is not "
           "supported");
    assign(Other, Allocator);
  }

  /// Copies values numbers and live segments from \p Other into this range.
  void assign(const CgLiveRange &Other, BumpPtrAllocator &Allocator) {
    if (this == &Other)
      return;

    assert(Other.segmentSet == nullptr &&
           "Copying of LiveRanges with active SegmentSets is not "
           "supported");
    // Duplicate valnos.
    for (const CgVNInfo *VNI : Other.valnos)
      createValueCopy(VNI, Allocator);
    // Now we can copy segments and remap their valnos.
    for (const Segment &S : Other.segments)
      segments.push_back(Segment(S.start, S.end, valnos[S.valno->id]));
  }

  /// advanceTo - Advance the specified iterator to point to the Segment
  /// containing the specified position, or end() if the position is past the
  /// end of the range.  If no Segment contains this position, but the
  /// position is in a hole, this method returns an iterator pointing to the
  /// Segment immediately after the hole.
  iterator advanceTo(iterator I, CgSlotIndex Pos) {
    assert(I != end());
    if (Pos >= endIndex())
      return end();
    while (I->end <= Pos)
      ++I;
    return I;
  }

  const_iterator advanceTo(const_iterator I, CgSlotIndex Pos) const {
    assert(I != end());
    if (Pos >= endIndex())
      return end();
    while (I->end <= Pos)
      ++I;
    return I;
  }

  /// find - Return an iterator pointing to the first segment that ends after
  /// Pos, or end(). This is the same as advanceTo(begin(), Pos), but faster
  /// when searching large ranges.
  ///
  /// If Pos is contained in a Segment, that segment is returned.
  /// If Pos is in a hole, the following Segment is returned.
  /// If Pos is beyond endIndex, end() is returned.
  iterator find(CgSlotIndex Pos);

  const_iterator find(CgSlotIndex Pos) const {
    return const_cast<CgLiveRange *>(this)->find(Pos);
  }

  void clear() {
    valnos.clear();
    segments.clear();
  }

  size_t size() const { return segments.size(); }

  bool hasAtLeastOneValue() const { return !valnos.empty(); }

  bool containsOneValue() const { return valnos.size() == 1; }

  unsigned getNumValNums() const { return (unsigned)valnos.size(); }

  /// getValNumInfo - Returns pointer to the specified val#.
  ///
  inline CgVNInfo *getValNumInfo(unsigned ValNo) { return valnos[ValNo]; }
  inline const CgVNInfo *getValNumInfo(unsigned ValNo) const {
    return valnos[ValNo];
  }

  /// containsValue - Returns true if VNI belongs to this range.
  bool containsValue(const CgVNInfo *VNI) const {
    return VNI && VNI->id < getNumValNums() && VNI == getValNumInfo(VNI->id);
  }

  /// getNextValue - Create a new value number and return it.  MIIdx specifies
  /// the instruction that defines the value number.
  CgVNInfo *getNextValue(CgSlotIndex def,
                         CgVNInfo::Allocator &VNInfoAllocator) {
    CgVNInfo *VNI =
        new (VNInfoAllocator) CgVNInfo((unsigned)valnos.size(), def);
    valnos.push_back(VNI);
    return VNI;
  }

  /// createDeadDef - Make sure the range has a value defined at Def.
  /// If one already exists, return it. Otherwise allocate a new value and
  /// add liveness for a dead def.
  CgVNInfo *createDeadDef(CgSlotIndex Def, CgVNInfo::Allocator &VNIAlloc);

  /// Create a def of value @p VNI. Return @p VNI. If there already exists
  /// a definition at VNI->def, the value defined there must be @p VNI.
  CgVNInfo *createDeadDef(CgVNInfo *VNI);

  /// Create a copy of the given value. The new value will be identical except
  /// for the Value number.
  CgVNInfo *createValueCopy(const CgVNInfo *orig,
                            CgVNInfo::Allocator &VNInfoAllocator) {
    CgVNInfo *VNI =
        new (VNInfoAllocator) CgVNInfo((unsigned)valnos.size(), *orig);
    valnos.push_back(VNI);
    return VNI;
  }

  /// RenumberValues - Renumber all values in order of appearance and remove
  /// unused values.
  void RenumberValues();

  /// MergeValueNumberInto - This method is called when two value numbers
  /// are found to be equivalent.  This eliminates V1, replacing all
  /// segments with the V1 value number with the V2 value number.  This can
  /// cause merging of V1/V2 values numbers and compaction of the value space.
  CgVNInfo *MergeValueNumberInto(CgVNInfo *V1, CgVNInfo *V2);

  /// Merge all of the live segments of a specific val# in RHS into this live
  /// range as the specified value number. The segments in RHS are allowed
  /// to overlap with segments in the current range, it will replace the
  /// value numbers of the overlaped live segments with the specified value
  /// number.
  void MergeSegmentsInAsValue(const CgLiveRange &RHS, CgVNInfo *LHSValNo);

  /// MergeValueInAsValue - Merge all of the segments of a specific val#
  /// in RHS into this live range as the specified value number.
  /// The segments in RHS are allowed to overlap with segments in the
  /// current range, but only if the overlapping segments have the
  /// specified value number.
  void MergeValueInAsValue(const CgLiveRange &RHS, const CgVNInfo *RHSValNo,
                           CgVNInfo *LHSValNo);

  bool empty() const { return segments.empty(); }

  /// beginIndex - Return the lowest numbered slot covered.
  CgSlotIndex beginIndex() const {
    assert(!empty() && "Call to beginIndex() on empty range.");
    return segments.front().start;
  }

  /// endNumber - return the maximum point of the range of the whole,
  /// exclusive.
  CgSlotIndex endIndex() const {
    assert(!empty() && "Call to endIndex() on empty range.");
    return segments.back().end;
  }

  bool expiredAt(CgSlotIndex index) const { return index >= endIndex(); }

  bool liveAt(CgSlotIndex index) const {
    const_iterator r = find(index);
    return r != end() && r->start <= index;
  }

  /// Return the segment that contains the specified index, or null if there
  /// is none.
  const Segment *getSegmentContaining(CgSlotIndex Idx) const {
    const_iterator I = FindSegmentContaining(Idx);
    return I == end() ? nullptr : &*I;
  }

  /// Return the live segment that contains the specified index, or null if
  /// there is none.
  Segment *getSegmentContaining(CgSlotIndex Idx) {
    iterator I = FindSegmentContaining(Idx);
    return I == end() ? nullptr : &*I;
  }

  /// getVNInfoAt - Return the CgVNInfo that is live at Idx, or NULL.
  CgVNInfo *getVNInfoAt(CgSlotIndex Idx) const {
    const_iterator I = FindSegmentContaining(Idx);
    return I == end() ? nullptr : I->valno;
  }

  /// getVNInfoBefore - Return the CgVNInfo that is live up to but not
  /// necessarily including Idx, or NULL. Use this to find the reaching def
  /// used by an instruction at this CgSlotIndex position.
  CgVNInfo *getVNInfoBefore(CgSlotIndex Idx) const {
    const_iterator I = FindSegmentContaining(Idx.getPrevSlot());
    return I == end() ? nullptr : I->valno;
  }

  /// Return an iterator to the segment that contains the specified index, or
  /// end() if there is none.
  iterator FindSegmentContaining(CgSlotIndex Idx) {
    iterator I = find(Idx);
    return I != end() && I->start <= Idx ? I : end();
  }

  const_iterator FindSegmentContaining(CgSlotIndex Idx) const {
    const_iterator I = find(Idx);
    return I != end() && I->start <= Idx ? I : end();
  }

  /// overlaps - Return true if the intersection of the two live ranges is
  /// not empty.
  bool overlaps(const CgLiveRange &other) const {
    if (other.empty())
      return false;
    return overlapsFrom(other, other.begin());
  }

  /// overlaps - Return true if the two ranges have overlapping segments
  /// that are not coalescable according to CP.
  ///
  /// Overlapping segments where one range is defined by a coalescable
  /// copy are allowed.
  bool overlaps(const CgLiveRange &Other, const CgCoalescerPair &CP,
                const CgSlotIndexes &) const;

  /// overlaps - Return true if the live range overlaps an interval specified
  /// by [Start, End).
  bool overlaps(CgSlotIndex Start, CgSlotIndex End) const;

  /// overlapsFrom - Return true if the intersection of the two live ranges
  /// is not empty.  The specified iterator is a hint that we can begin
  /// scanning the Other range starting at I.
  bool overlapsFrom(const CgLiveRange &Other, const_iterator StartPos) const;

  /// Returns true if all segments of the @p Other live range are completely
  /// covered by this live range.
  /// Adjacent live ranges do not affect the covering:the liverange
  /// [1,5](5,10] covers (3,7].
  bool covers(const CgLiveRange &Other) const;

  /// Add the specified Segment to this range, merging segments as
  /// appropriate.  This returns an iterator to the inserted segment (which
  /// may have grown since it was inserted).
  iterator addSegment(Segment S);

  /// Attempt to extend a value defined after @p StartIdx to include @p Use.
  /// Both @p StartIdx and @p Use should be in the same basic block. In case
  /// of subranges, an extension could be prevented by an explicit "undef"
  /// caused by a <def,read-undef> on a non-overlapping lane. The list of
  /// location of such "undefs" should be provided in @p Undefs.
  /// The return value is a pair: the first element is CgVNInfo of the value
  /// that was extended (possibly nullptr), the second is a boolean value
  /// indicating whether an "undef" was encountered.
  /// If this range is live before @p Use in the basic block that starts at
  /// @p StartIdx, and there is no intervening "undef", extend it to be live
  /// up to @p Use, and return the pair {value, false}. If there is no
  /// segment before @p Use and there is no "undef" between @p StartIdx and
  /// @p Use, return {nullptr, false}. If there is an "undef" before @p Use,
  /// return {nullptr, true}.
  std::pair<CgVNInfo *, bool> extendInBlock(ArrayRef<CgSlotIndex> Undefs,
                                            CgSlotIndex StartIdx,
                                            CgSlotIndex Kill);

  /// Simplified version of the above "extendInBlock", which assumes that
  /// no register lanes are undefined by <def,read-undef> operands.
  /// If this range is live before @p Use in the basic block that starts
  /// at @p StartIdx, extend it to be live up to @p Use, and return the
  /// value. If there is no segment before @p Use, return nullptr.
  CgVNInfo *extendInBlock(CgSlotIndex StartIdx, CgSlotIndex Kill);

  /// join - Join two live ranges (this, and other) together.  This applies
  /// mappings to the value numbers in the LHS/RHS ranges as specified.  If
  /// the ranges are not joinable, this aborts.
  void join(CgLiveRange &Other, const int *ValNoAssignments,
            const int *RHSValNoAssignments,
            SmallVectorImpl<CgVNInfo *> &NewVNInfo);

  /// True iff this segment is a single segment that lies between the
  /// specified boundaries, exclusively. Vregs live across a backedge are not
  /// considered local. The boundaries are expected to lie within an extended
  /// basic block, so vregs that are not live out should contain no holes.
  bool isLocal(CgSlotIndex Start, CgSlotIndex End) const {
    return beginIndex() > Start.getBaseIndex() &&
           endIndex() < End.getBoundaryIndex();
  }

  /// Remove the specified segment from this range.  Note that the segment
  /// must be a single Segment in its entirety.
  void removeSegment(CgSlotIndex Start, CgSlotIndex End,
                     bool RemoveDeadValNo = false);

  void removeSegment(Segment S, bool RemoveDeadValNo = false) {
    removeSegment(S.start, S.end, RemoveDeadValNo);
  }

  /// Remove segment pointed to by iterator @p I from this range.
  iterator removeSegment(iterator I, bool RemoveDeadValNo = false);

  /// Mark \p ValNo for deletion if no segments in this range use it.
  void removeValNoIfDead(CgVNInfo *ValNo);

  /// Query Liveness at Idx.
  /// The sub-instruction slot of Idx doesn't matter, only the instruction
  /// it refers to is considered.
  CgLiveQueryResult Query(CgSlotIndex Idx) const {
    // Find the segment that enters the instruction.
    const_iterator I = find(Idx.getBaseIndex());
    const_iterator E = end();
    if (I == E)
      return CgLiveQueryResult(nullptr, nullptr, CgSlotIndex(), false);

    // Is this an instruction live-in segment?
    // If Idx is the start index of a basic block, include live-in segments
    // that start at Idx.getBaseIndex().
    CgVNInfo *EarlyVal = nullptr;
    CgVNInfo *LateVal = nullptr;
    CgSlotIndex EndPoint;
    bool Kill = false;
    if (I->start <= Idx.getBaseIndex()) {
      EarlyVal = I->valno;
      EndPoint = I->end;
      // Move to the potentially live-out segment.
      if (CgSlotIndex::isSameInstr(Idx, I->end)) {
        Kill = true;
        if (++I == E)
          return CgLiveQueryResult(EarlyVal, LateVal, EndPoint, Kill);
      }
      // Special case: A PHIDef value can have its def in the middle of a
      // segment if the value happens to be live out of the layout
      // predecessor.
      // Such a value is not live-in.
      if (EarlyVal->def == Idx.getBaseIndex())
        EarlyVal = nullptr;
    }
    // I now points to the segment that may be live-through, or defined by
    // this instr. Ignore segments starting after the current instr.
    if (!CgSlotIndex::isEarlierInstr(Idx, I->start)) {
      LateVal = I->valno;
      EndPoint = I->end;
    }
    return CgLiveQueryResult(EarlyVal, LateVal, EndPoint, Kill);
  }

  /// removeValNo - Remove all the segments defined by the specified value#.
  /// Also remove the value# from value# list.
  void removeValNo(CgVNInfo *ValNo);

  /// Returns true if the live range is zero length, i.e. no live segments
  /// span instructions. It doesn't pay to spill such a range.
  bool isZeroLength(CgSlotIndexes *Indexes) const {
    for (const Segment &S : segments)
      if (Indexes->getNextNonNullIndex(S.start).getBaseIndex() <
          S.end.getBaseIndex())
        return false;
    return true;
  }

  // Returns true if any segment in the live range contains any of the
  // provided slot indexes.  Slots which occur in holes between
  // segments will not cause the function to return true.
  bool isLiveAtIndexes(ArrayRef<CgSlotIndex> Slots) const;

  bool operator<(const CgLiveRange &other) const {
    const CgSlotIndex &thisIndex = beginIndex();
    const CgSlotIndex &otherIndex = other.beginIndex();
    return thisIndex < otherIndex;
  }

  /// Returns true if there is an explicit "undef" between @p Begin
  /// @p End.
  bool isUndefIn(ArrayRef<CgSlotIndex> Undefs, CgSlotIndex Begin,
                 CgSlotIndex End) const {
    return llvm::any_of(Undefs, [Begin, End](CgSlotIndex Idx) -> bool {
      return Begin <= Idx && Idx < End;
    });
  }

  /// Flush segment set into the regular segment vector.
  /// The method is to be called after the live range
  /// has been created, if use of the segment set was
  /// activated in the constructor of the live range.
  void flushSegmentSet();

  /// Stores indexes from the input index sequence R at which this CgLiveRange
  /// is live to the output O iterator.
  /// R is a range of _ascending sorted_ _random_ access iterators
  /// to the input indexes. Indexes stored at O are ascending sorted so it
  /// can be used directly in the subsequent search (for example for
  /// subranges). Returns true if found at least one index.
  template <typename Range, typename OutputIt>
  bool findIndexesLiveAt(Range &&R, OutputIt O) const {
    assert(llvm::is_sorted(R));
    auto Idx = R.begin(), EndIdx = R.end();
    auto Seg = segments.begin(), EndSeg = segments.end();
    bool Found = false;
    while (Idx != EndIdx && Seg != EndSeg) {
      // if the Seg is lower find first segment that is above Idx using
      // binary search
      if (Seg->end <= *Idx) {
        Seg = std::upper_bound(++Seg, EndSeg, *Idx, [=](auto V, const auto &S) {
          return V < S.end;
        });
        if (Seg == EndSeg)
          break;
      }
      auto NotLessStart = std::lower_bound(Idx, EndIdx, Seg->start);
      if (NotLessStart == EndIdx)
        break;
      auto NotLessEnd = std::lower_bound(NotLessStart, EndIdx, Seg->end);
      if (NotLessEnd != NotLessStart) {
        Found = true;
        O = std::copy(NotLessStart, NotLessEnd, O);
      }
      Idx = NotLessEnd;
      ++Seg;
    }
    return Found;
  }

  void print(raw_ostream &OS) const;
  void dump() const;

  /// Walk the range and assert if any invariants fail to hold.
  ///
  /// Note that this is a no-op when asserts are disabled.
#ifdef NDEBUG
  void verify() const {}
#else
  void verify() const;
#endif

protected:
  /// Append a segment to the list of segments.
  void append(const CgLiveRange::Segment S);

private:
  friend class CgLiveRangeUpdater;
  void addSegmentToSet(Segment S);
  void markValNoForDeletion(CgVNInfo *V);
};

/// CgLiveInterval - This class represents the liveness of a register,
/// or stack slot.
class CgLiveInterval : public CgLiveRange {
public:
  using super = CgLiveRange;

  /// A live range for subregisters. The LaneMask specifies which parts of the
  /// super register are covered by the interval.
  /// (@sa TargetRegisterInfo::getSubRegIndexLaneMask()).
  class SubRange : public CgLiveRange {
  public:
    SubRange *Next = nullptr;
    LaneBitmask LaneMask;

    /// Constructs a new SubRange object.
    SubRange(LaneBitmask LaneMask) : LaneMask(LaneMask) {}

    /// Constructs a new SubRange object by copying liveness from @p Other.
    SubRange(LaneBitmask LaneMask, const CgLiveRange &Other,
             BumpPtrAllocator &Allocator)
        : CgLiveRange(Other, Allocator), LaneMask(LaneMask) {}

    void print(raw_ostream &OS) const;
    void dump() const;
  };

private:
  SubRange *SubRanges = nullptr; ///< Single linked list of subregister live
                                 /// ranges.
  const Register Reg;            // the register or stack slot of this interval.
  float Weight = 0.0;            // weight of this interval

public:
  Register reg() const { return Reg; }
  float weight() const { return Weight; }
  void incrementWeight(float Inc) { Weight += Inc; }
  void setWeight(float Value) { Weight = Value; }

  CgLiveInterval(unsigned Reg, float Weight) : Reg(Reg), Weight(Weight) {}

  ~CgLiveInterval() { clearSubRanges(); }

  template <typename T> class SingleLinkedListIterator {
    T *P;

  public:
    SingleLinkedListIterator(T *P) : P(P) {}

    SingleLinkedListIterator<T> &operator++() {
      P = P->Next;
      return *this;
    }
    SingleLinkedListIterator<T> operator++(int) {
      SingleLinkedListIterator res = *this;
      ++*this;
      return res;
    }
    bool operator!=(const SingleLinkedListIterator<T> &Other) const {
      return P != Other.operator->();
    }
    bool operator==(const SingleLinkedListIterator<T> &Other) const {
      return P == Other.operator->();
    }
    T &operator*() const { return *P; }
    T *operator->() const { return P; }
  };

  using subrange_iterator = SingleLinkedListIterator<SubRange>;
  using const_subrange_iterator = SingleLinkedListIterator<const SubRange>;

  subrange_iterator subrange_begin() { return subrange_iterator(SubRanges); }
  subrange_iterator subrange_end() { return subrange_iterator(nullptr); }

  const_subrange_iterator subrange_begin() const {
    return const_subrange_iterator(SubRanges);
  }
  const_subrange_iterator subrange_end() const {
    return const_subrange_iterator(nullptr);
  }

  iterator_range<subrange_iterator> subranges() {
    return make_range(subrange_begin(), subrange_end());
  }

  iterator_range<const_subrange_iterator> subranges() const {
    return make_range(subrange_begin(), subrange_end());
  }

  /// Creates a new empty subregister live range. The range is added at the
  /// beginning of the subrange list; subrange iterators stay valid.
  SubRange *createSubRange(BumpPtrAllocator &Allocator, LaneBitmask LaneMask) {
    SubRange *Range = new (Allocator) SubRange(LaneMask);
    appendSubRange(Range);
    return Range;
  }

  /// Like createSubRange() but the new range is filled with a copy of the
  /// liveness information in @p CopyFrom.
  SubRange *createSubRangeFrom(BumpPtrAllocator &Allocator,
                               LaneBitmask LaneMask,
                               const CgLiveRange &CopyFrom) {
    SubRange *Range = new (Allocator) SubRange(LaneMask, CopyFrom, Allocator);
    appendSubRange(Range);
    return Range;
  }

  /// Returns true if subregister liveness information is available.
  bool hasSubRanges() const { return SubRanges != nullptr; }

  /// Removes all subregister liveness information.
  void clearSubRanges();

  /// Removes all subranges without any segments (subranges without segments
  /// are not considered valid and should only exist temporarily).
  void removeEmptySubRanges();

  /// getSize - Returns the sum of sizes of all the CgLiveRange's.
  ///
  unsigned getSize() const;

  /// isSpillable - Can this interval be spilled?
  bool isSpillable() const {
    return std::fabs(Weight - huge_valf) >
           std::numeric_limits<float>::epsilon();
  }

  /// markNotSpillable - Mark interval as not spillable
  void markNotSpillable() { Weight = huge_valf; }

  /// For a given lane mask @p LaneMask, compute indexes at which the
  /// lane is marked undefined by subregister <def,read-undef> definitions.
  void computeSubRangeUndefs(SmallVectorImpl<CgSlotIndex> &Undefs,
                             LaneBitmask LaneMask, const CgRegisterInfo &MRI,
                             const CgSlotIndexes &Indexes) const;

  /// Refines the subranges to support \p LaneMask. This may only be called
  /// for LI.hasSubrange()==true. Subregister ranges are split or created
  /// until \p LaneMask can be matched exactly. \p Mod is executed on the
  /// matching subranges.
  ///
  /// Example:
  ///    Given an interval with subranges with lanemasks L0F00, L00F0 and
  ///    L000F, refining for mask L0018. Will split the L00F0 lane into
  ///    L00E0 and L0010 and the L000F lane into L0007 and L0008. The Mod
  ///    function will be applied to the L0010 and L0008 subranges.
  ///
  /// \p Indexes and \p TRI are required to clean up the VNIs that
  /// don't define the related lane masks after they get shrunk. E.g.,
  /// when L000F gets split into L0007 and L0008 maybe only a subset
  /// of the VNIs that defined L000F defines L0007.
  ///
  /// The clean up of the VNIs need to look at the actual instructions
  /// to decide what is or is not live at a definition point. If the
  /// update of the subranges occurs while the IR does not reflect these
  /// changes, \p ComposeSubRegIdx can be used to specify how the
  /// definition are going to be rewritten.
  /// E.g., let say we want to merge:
  ///     V1.sub1:<2 x s32> = COPY V2.sub3:<4 x s32>
  /// We do that by choosing a class where sub1:<2 x s32> and sub3:<4 x s32>
  /// overlap, i.e., by choosing a class where we can find "offset + 1 == 3".
  /// Put differently we align V2's sub3 with V1's sub1:
  /// V2: sub0 sub1 sub2 sub3
  /// V1: <offset>  sub0 sub1
  ///
  /// This offset will look like a composed subregidx in the the class:
  ///     V1.(composed sub2 with sub1):<4 x s32> = COPY V2.sub3:<4 x s32>
  /// =>  V1.(composed sub2 with sub1):<4 x s32> = COPY V2.sub3:<4 x s32>
  ///
  /// Now if we didn't rewrite the uses and def of V1, all the checks for V1
  /// need to account for this offset.
  /// This happens during coalescing where we update the live-ranges while
  /// still having the old IR around because updating the IR on-the-fly
  /// would actually clobber some information on how the live-ranges that
  /// are being updated look like.
  void refineSubRanges(BumpPtrAllocator &Allocator, LaneBitmask LaneMask,
                       std::function<void(CgLiveInterval::SubRange &)> Apply,
                       const CgSlotIndexes &Indexes,
                       const TargetRegisterInfo &TRI,
                       unsigned ComposeSubRegIdx = 0);

  bool operator<(const CgLiveInterval &other) const {
    const CgSlotIndex &thisIndex = beginIndex();
    const CgSlotIndex &otherIndex = other.beginIndex();
    return std::tie(thisIndex, Reg) < std::tie(otherIndex, other.Reg);
  }

  void print(raw_ostream &OS) const;
  void dump() const;

  /// Walks the interval and assert if any invariants fail to hold.
  ///
  /// Note that this is a no-op when asserts are disabled.
#ifdef NDEBUG
  void verify(const CgRegisterInfo *MRI = nullptr) const {}
#else
  void verify(const CgRegisterInfo *MRI = nullptr) const;
#endif

private:
  /// Appends @p Range to SubRanges list.
  void appendSubRange(SubRange *Range) {
    Range->Next = SubRanges;
    SubRanges = Range;
  }

  /// Free memory held by SubRange.
  void freeSubRange(SubRange *S);
};

inline bool operator<(CgSlotIndex V, const CgLiveRange::Segment &S) {
  return V < S.start;
}

inline bool operator<(const CgLiveRange::Segment &S, CgSlotIndex V) {
  return S.start < V;
}

/// Helper class for performant CgLiveRange bulk updates.
///
/// Calling CgLiveRange::addSegment() repeatedly can be expensive on large
/// live ranges because segments after the insertion point may need to be
/// shifted. The CgLiveRangeUpdater class can defer the shifting when adding
/// many segments in order.
///
/// The CgLiveRange will be in an invalid state until flush() is called.
class CgLiveRangeUpdater {
  CgLiveRange *LR = nullptr;
  CgSlotIndex LastStart;
  CgLiveRange::iterator WriteI{};
  CgLiveRange::iterator ReadI{};
  SmallVector<CgLiveRange::Segment, 16> Spills;
  void mergeSpills();

public:
  /// Create a CgLiveRangeUpdater for adding segments to LR.
  /// LR will temporarily be in an invalid state until flush() is called.
  CgLiveRangeUpdater(CgLiveRange *lr = nullptr) : LR(lr) {}

  ~CgLiveRangeUpdater() { flush(); }

  /// Add a segment to LR and coalesce when possible, just like
  /// LR.addSegment(). Segments should be added in increasing start order for
  /// best performance.
  void add(CgLiveRange::Segment);

  void add(CgSlotIndex Start, CgSlotIndex End, CgVNInfo *VNI) {
    add(CgLiveRange::Segment(Start, End, VNI));
  }

  /// Return true if the LR is currently in an invalid state, and flush()
  /// needs to be called.
  bool isDirty() const { return LastStart.isValid(); }

  /// Flush the updater state to LR so it is valid and contains all added
  /// segments.
  void flush();

  /// Select a different destination live range.
  void setDest(CgLiveRange *lr) {
    if (LR != lr && isDirty())
      flush();
    LR = lr;
  }

  /// Get the current destination live range.
  CgLiveRange *getDest() const { return LR; }

  void dump() const;
  void print(raw_ostream &) const;
};

/// ConnectedVNInfoEqClasses - Helper class that can divide VNInfos in a
/// CgLiveInterval into equivalence clases of connected components. A
/// CgLiveInterval that has multiple connected components can be broken into
/// multiple CgLiveIntervals.
///
/// Given a CgLiveInterval that may have multiple connected components, run:
///
///   unsigned numComps = ConEQ.Classify(LI);
///   if (numComps > 1) {
///     // allocate numComps-1 new CgLiveIntervals into LIS[1..]
///     ConEQ.Distribute(LIS);
/// }

class ConnectedVNInfoEqClasses {
  CgLiveIntervals &LIS;
  IntEqClasses EqClass;

public:
  explicit ConnectedVNInfoEqClasses(CgLiveIntervals &lis) : LIS(lis) {}

  /// Classify the values in \p LR into connected components.
  /// Returns the number of connected components.
  unsigned Classify(const CgLiveRange &LR);

  /// getEqClass - Classify creates equivalence classes numbered 0..N. Return
  /// the equivalence class assigned the VNI.
  unsigned getEqClass(const CgVNInfo *VNI) const { return EqClass[VNI->id]; }

  /// Distribute values in \p LI into a separate CgLiveIntervals
  /// for each connected component. LIV must have an empty CgLiveInterval for
  /// each additional connected component. The first connected component is
  /// left in \p LI.
  void Distribute(CgLiveInterval &LI, CgLiveInterval *LIV[],
                  CgRegisterInfo &MRI);
};

} // namespace COMPILER

namespace llvm {
inline raw_ostream &operator<<(raw_ostream &OS, const CgLiveRangeUpdater &X) {
  X.print(OS);
  return OS;
}
inline raw_ostream &operator<<(raw_ostream &OS,
                               const CgLiveInterval::SubRange &SR) {
  SR.print(OS);
  return OS;
}

inline raw_ostream &operator<<(raw_ostream &OS, const CgLiveInterval &LI) {
  LI.print(OS);
  return OS;
}

raw_ostream &operator<<(raw_ostream &OS, const CgLiveRange::Segment &S);

inline raw_ostream &operator<<(raw_ostream &OS, const CgLiveRange &LR) {
  LR.print(OS);
  return OS;
}
} // namespace llvm
