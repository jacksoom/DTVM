load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_CLI11_deps():
    http_archive(
        name = "zen_deps_CLI11",
        build_file = "//third_party/CLI11:CLI11.BUILD",
        # branch = "main",
        # commit = "ff648c81ee9278b5b8e1367f6a24237dabeb3942",
        sha256 = "43e650d5e1a3acaaf419d1e61a81f77b408d0696f472be0599ddf877d40984b0",
        strip_prefix = "CLI11-2.4.2",
        urls = [
            "https://github.com/CLIUtils/CLI11/archive/v2.4.2.zip",
        ],
    )
