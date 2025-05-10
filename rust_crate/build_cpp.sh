#!/bin/bash
# cd .. # <--- Comment out or remove this line
if [ "$1" != "cache" ]; then
    rm -rf bazel-bin bazel-out bazel-testlogs bazel-dtvm
    bazel clean --expunge
fi

# for bazel 8
# bash build.sh release.lib

# for bazel 1.2.1
# Ensure these lines use the //:target_name syntax as previously discussed:
CXXFLAGS="-fPIE" CFLAGS="-fPIE" bazel build //:zetaengine --copt="-fPIE"  --compilation_mode=opt --jobs=16 --verbose_failures
CXXFLAGS="-fPIE" CFLAGS="-fPIE" bazel build //:utils_lib --copt="-fPIE"  --compilation_mode=opt --jobs=16 --verbose_failures
CXXFLAGS="-fPIE" CFLAGS="-fPIE" bazel build @zen_deps_asmjit//:asmjit --copt="-fPIE"  --compilation_mode=opt --jobs=16 --verbose_failures
