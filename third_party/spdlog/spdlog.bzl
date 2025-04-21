load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_spdlog_deps():
    http_archive(
        name = "zen_deps_spdlog",
        build_file = "//third_party/spdlog:spdlog.BUILD",
        sha256 = "821c85b120ad15d87ca2bc44185fa9091409777c756029125a02f81354072157",
        strip_prefix = "spdlog-1.4.2",
        urls = [
            "https://github.com/gabime/spdlog/archive/v1.4.2.zip",
        ],
    )
