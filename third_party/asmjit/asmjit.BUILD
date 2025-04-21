config_setting(
    name = "linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "macos",
    constraint_values = [
        "@platforms//os:osx",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "asmjit",
    srcs = glob([
        "src/asmjit/core/*.h",
        "src/asmjit/core/*.cpp",
    ]) + select({
        ":x86_64": glob([
            "src/asmjit/x86/*.h",
            "src/asmjit/x86/*.cpp",
        ]),
        ":aarch64": glob([
            "src/asmjit/arm/*.h",
            "src/asmjit/arm/*.cpp",
        ]),
    }),
    hdrs = glob([
        "src/asmjit/asmjit.h",
        "src/asmjit/asmjit-scope-begin.h",
        "src/asmjit/asmjit-scope-end.h",
        "src/asmjit/core.h",
        "src/asmjit/core/*.h",
    ]) + select({
        ":x86_64": glob([
            "src/asmjit/x86.h",
            "src/asmjit/x86/*.h",
        ]),
        ":aarch64": glob([
            "src/asmjit/arm.h",
            "src/asmjit/a64.h",
            "src/asmjit/arm/*.h",
        ]),
    }),
    linkstatic = True,
    local_defines = [
        "ASMJIT_STATIC",
        "ASMJIT_NO_FOREIGN",
        "ASMJIT_NO_DEPRECATED",
        "ASMJIT_NO_BUILDER",
        "ASMJIT_NO_COMPILER",
        "ASMJIT_NO_JIT",
        "ASMJIT_NO_LOGGING",
        "ASMJIT_NO_INTROSPECTION",
    ] + select({
        ":x86_64": [
            "ASMJIT_NO_AARCH64",
        ],
        ":aarch64": [
            "ASMJIT_NO_X86",
        ],
    }),
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)
