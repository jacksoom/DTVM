
//===-------- CgEdgeBundles.cpp - Bundles of CFG edges
//----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides the implementation of the CgEdgeBundles analysis.
//
//===----------------------------------------------------------------------===//

#include "edge_bundles.h"
#include "llvm/ADT/Twine.h"

#include "cgir/cg_basic_block.h"
#include "cgir/cg_function.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace COMPILER;

static bool ViewEdgeBundles = false;

bool CgEdgeBundles::runOnCgFunction(CgFunction &mf) {
  MF = &mf;
  EC.clear();
  EC.grow(2 * MF->getNumBlockIDs());

  for (const auto *MBB : *MF) {
    unsigned OutE = 2 * MBB->getNumber() + 1;
    // Join the outgoing bundle with the ingoing bundles of all successors.
    for (const CgBasicBlock *Succ : MBB->successors())
      EC.join(OutE, 2 * Succ->getNumber());
  }
  EC.compress();
  // if (ViewEdgeBundles)
  //   view();

  // Compute the reverse mapping.
  Blocks.clear();
  Blocks.resize(getNumBundles());

  for (unsigned i = 0, e = MF->getNumBlockIDs(); i != e; ++i) {
    unsigned b0 = getBundle(i, false);
    unsigned b1 = getBundle(i, true);
    Blocks[b0].push_back(i);
    if (b1 != b0)
      Blocks[b1].push_back(i);
  }

  MF->EdgeBundles = this;

  return false;
}

#if 0
namespace llvm {

/// Specialize WriteGraph, the standard implementation won't work.
template<>
raw_ostream &WriteGraph<>(raw_ostream &O, const CgEdgeBundles &G,
                          bool ShortNames,
                          const Twine &Title) {
  const CgFunction *MF = G.getMachineFunction();

  O << "digraph {\n";
  for (const auto &MBB : *MF) {
    unsigned BB = MBB.getNumber();
    O << "\t\"" << printCgBBReference(MBB) << "\" [ shape=box ]\n"
      << '\t' << G.getBundle(BB, false) << " -> \"" << printCgBBReference(MBB)
      << "\"\n"
      << "\t\"" << printCgBBReference(MBB) << "\" -> " << G.getBundle(BB, true)
      << '\n';
    for (const CgBasicBlock *Succ : MBB.successors())
      O << "\t\"" << printCgBBReference(MBB) << "\" -> \""
        << printCgBBReference(*Succ) << "\" [ color=lightgray ]\n";
  }
  O << "}\n";
  return O;
}

} // end namespace llvm

/// view - Visualize the annotated bipartite CFG with Graphviz.
void CgEdgeBundles::view() const {
  ViewGraph(*this, "CgEdgeBundles");
}
#endif
