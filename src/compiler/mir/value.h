// Copyright (C) 2021-2023 Ant Group Co., Ltd. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0
#ifndef COMPILER_IR_VALUE_H
#define COMPILER_IR_VALUE_H

namespace COMPILER {

class Value : public NonCopyable {
public:
  enum Kind : uint8_t {
    ARGUMENT,
    VAR,
    EXPRESSION,
  };

protected:
  Kind _kind : 8;
  unsigned _subClassData : 24;
};

} // namespace COMPILER

#endif // COMPILER_IR_VALUE_H
