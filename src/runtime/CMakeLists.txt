# Copyright (C) 2021-2023 Ant Group Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(RUNTIME_SRCS
    vnmi.cpp
    wni.cpp
    runtime.cpp
    isolation.cpp
    module.cpp
    instance.cpp
    codeholder.cpp
    destroyer.cpp
    memory.cpp
)

add_library(runtime OBJECT ${RUNTIME_SRCS})
