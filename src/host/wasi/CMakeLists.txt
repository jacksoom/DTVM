set(WASI_SRCS
    wasi.cpp
    sandboxed-system-primitives/src/posix.c
    sandboxed-system-primitives/src/random.c
    sandboxed-system-primitives/src/str.c
)

include_directories(sandboxed-system-primitives/include)

add_library(host_wasi OBJECT ${WASI_SRCS})
