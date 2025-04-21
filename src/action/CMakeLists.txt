# Copyright (C) 2021-2023 Ant Group Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(ACTION_SRCS instantiator.cpp interpreter.cpp compiler.cpp module_loader.cpp
                function_loader.cpp
)

add_library(action OBJECT ${ACTION_SRCS})
