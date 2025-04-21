cc_library(
    name = "spdlog",
    srcs = glob([
        "src/*.cpp",
        "include/**/*.cc",
        "include/spdlog/**/*.h",
        "include/spdlog/*.h",
    ]),
    hdrs = glob([
        "include/**/*.cc",
        "include/spdlog/**/*.h",
        "include/spdlog/*.h",
    ]),
    defines = [
        "SPDLOG_COMPILED_LIB",
    ],
    includes = [
        "include",
        "src",
    ],
    visibility = ["//visibility:public"],
)
