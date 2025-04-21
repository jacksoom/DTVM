// Copyright (C) 2021-2023 Ant Group Co., Ltd. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef ZEN_ZETAENGINE_H
#define ZEN_ZETAENGINE_H

#include "common/defines.h"
#include "common/errors.h"
#include "runtime/instance.h"
#include "runtime/isolation.h"
#include "runtime/module.h"
#include "runtime/runtime.h"
#include "utils/logging.h"
#include "wni/helper.h"

namespace zen {

inline void setGlobalLogger(std::shared_ptr<utils::ILogger> Logger) {
  utils::Logging::getInstance().setLogger(Logger);
}

} // namespace zen

#endif // ZEN_ZETAENGINE_H
