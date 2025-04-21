//===-------- CgEdgeBundles.h - Bundles of CFG edges --------------*- c++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The CgEdgeBundles analysis forms equivalence classes of CFG edges such that
// all edges leaving a machine basic block are in the same bundle, and all edges
// entering a machine basic block are in the same bundle.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common_defs.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/IntEqClasses.h"

using namespace llvm;

namespace COMPILER {
class CgFunction;

class CgEdgeBundles : public NonCopyable {
  CgFunction *MF;

  /// EC - Each edge bundle is an equivalence class. The keys are:
  ///   2*BB->getNumber()   -> Ingoing bundle.
  ///   2*BB->getNumber()+1 -> Outgoing bundle.
  IntEqClasses EC;

  /// Blocks - Map each bundle to a list of basic block numbers.
  SmallVector<SmallVector<unsigned, 8>, 4> Blocks;

public:
  CgEdgeBundles(CgFunction &MF) { runOnCgFunction(MF); }

  /// getBundle - Return the ingoing (Out = false) or outgoing (Out = true)
  /// bundle number for basic block #N
  unsigned getBundle(unsigned N, bool Out) const { return EC[2 * N + Out]; }

  /// getNumBundles - Return the total number of bundles in the CFG.
  unsigned getNumBundles() const { return EC.getNumClasses(); }

  /// getBlocks - Return an array of blocks that are connected to Bundle.
  ArrayRef<unsigned> getBlocks(unsigned Bundle) const { return Blocks[Bundle]; }

  /// getMachineFunction - Return the last machine function computed.
  CgFunction *getMachineFunction() const { return MF; }

  /// view - Visualize the annotated bipartite CFG with Graphviz.
  // void view() const;

private:
  bool runOnCgFunction(CgFunction &);
};

} // end namespace COMPILER
