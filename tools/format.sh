#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

set -e

# Check if clang-format is installed
if ! command -v clang-format &>/dev/null; then
  echo "Error: clang-format command not found, please install it and try again." >&2
  exit 1
fi

# Check if cmake-format is installed
if ! command -v cmake-format &>/dev/null; then
  echo "Error: cmake-format command not found, please install it and try again." >&2
  exit 1
fi

if [ "$1" == "check" ]; then
  # Check the format of all CMake files
  cmake-format --check CMakeLists.txt
  find src -type f -name "CMakeLists.txt" | xargs cmake-format --check
  find third_party -type f -name "*.cmake" | xargs cmake-format --check
  # Check the format of all C/C++ files
  find src -path "src/compiler/llvm-prebuild" -prune -type f -or -name "*.h" -or -name "*.c" -or -name "*.cpp" | xargs clang-format --dry-run -style=file -Werror
elif [ "$1" == "format" ]; then
  # Format all CMake files
  cmake-format -i CMakeLists.txt
  find src -type f -name "CMakeLists.txt" | xargs cmake-format -i
  find third_party -type f -name "*.cmake" | xargs cmake-format -i
  # Format all C/C++ files
  find src -path "src/compiler/llvm-prebuild" -prune -type f -or -name "*.h" -or -name "*.c" -or -name "*.cpp" | xargs clang-format -i -style=file -Werror
else
  echo "Error: invalid argument"
  exit 1
fi
