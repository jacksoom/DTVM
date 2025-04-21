
//===- llvm/CodeGen/TileShapeInfo.h - ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file Shape utility for AMX.
/// AMX hardware requires to config the shape of tile data register before use.
/// The 2D shape includes row and column. In AMX intrinsics interface the shape
/// is passed as 1st and 2nd parameter and they are lowered as the 1st and 2nd
/// machine operand of AMX pseudo instructions. CgShapeT class is to facilitate
/// tile config and register allocator. The row and column are machine operand
/// of AMX pseudo instructions.
//
//===----------------------------------------------------------------------===//
#pragma once

#include "compiler/cgir/cg_instruction.h"
#include "compiler/cgir/cg_operand.h"
#include "compiler/cgir/pass/cg_register_info.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/CodeGen/Register.h"

namespace COMPILER {

class CgShapeT {
public:
  CgShapeT(CgOperand *Row, CgOperand *Col, const CgRegisterInfo *MRI = nullptr)
      : Row(Row), Col(Col) {
    if (MRI)
      deduceImm(MRI);
  }
  CgShapeT()
      : Row(nullptr), Col(nullptr), RowImm(InvalidImmShape),
        ColImm(InvalidImmShape) {}
  bool operator==(const CgShapeT &Shape) const {
    CgOperand *R = Shape.Row;
    CgOperand *C = Shape.Col;
    if (!R || !C)
      return false;
    if (!Row || !Col)
      return false;
    if (Row->getReg() == R->getReg() && Col->getReg() == C->getReg())
      return true;
    if ((RowImm != InvalidImmShape) && (ColImm != InvalidImmShape))
      return RowImm == Shape.getRowImm() && ColImm == Shape.getColImm();
    return false;
  }

  bool operator!=(const CgShapeT &Shape) const { return !(*this == Shape); }

  CgOperand *getRow() const { return Row; }

  CgOperand *getCol() const { return Col; }

  int64_t getRowImm() const { return RowImm; }

  int64_t getColImm() const { return ColImm; }

  bool isValid() { return (Row != nullptr) && (Col != nullptr); }

  void deduceImm(const CgRegisterInfo *MRI) {
    // All def must be the same value, otherwise it is invalid MIs.
    // Find the immediate.
    // TODO copy propagation.
    auto GetImm = [&](Register Reg) {
      int64_t Imm = InvalidImmShape;
      for (const CgOperand &DefMO : MRI->def_operands(Reg)) {
        const auto *MI = DefMO.getParent();
        if (MI->isMoveImmediate()) {
          Imm = MI->getOperand(1).getImm();
          break;
        }
      }
      return Imm;
    };
    RowImm = GetImm(Row->getReg());
    ColImm = GetImm(Col->getReg());
  }

private:
  static constexpr int64_t InvalidImmShape = -1;
  CgOperand *Row = nullptr;
  CgOperand *Col = nullptr;
  int64_t RowImm = InvalidImmShape;
  int64_t ColImm = InvalidImmShape;
};

} // end namespace COMPILER
