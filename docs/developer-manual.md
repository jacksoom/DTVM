# 1. Overview
This document aims to introduce developers to some important classes and interfaces of ZetaEngine, helping them quickly familiarize themselves with the internal code of ZetaEngine before submitting code.

# 2. Quick Development and Build
```shell
# Clone the repository
git clone <path-of-this-repo>

# Enter the project directory
cd <code-dir>

# Pull git submodule dependencies
git submodule update --init

# Apply patch for spec test cases
pushd tests/wast/spec && git apply ../spec.patch && popd

# CMake configuration & build
cmake -S . -B build -DZEN_ENABLE_ASAN=ON -DZEN_ENABLE_SINGLEPASS_JIT=ON -DZEN_ENABLE_SPEC_TEST=ON
cmake --build build -j 64
```

# 3. Recommended Environment & Coding Standards

### Recommended Development Environment
- Operating System: Ubuntu 20.04, Ubuntu 22.04, CentOS 7
- Compiler: gcc 7.3.x, gcc 9.4.x

### Coding Standards
- C++ and STL Version: C++17
- Code Style: [LLVM Style](https://llvm.org/docs/CodingStandards.html)
   - Naming Convention: Types/Classes/Enums/Variables use PascalCase, Functions use camelCase
   - Code Indentation and Line Limit: 2-space indentation, 80-character line limit
   - Header Guard Macro: `#ifdef`

# 4. Directory Structure and Modules
```
DTWasm
├── docs/         # Project documentation
├── examples/     # Usage examples
├── rust_crate/   # Rust SDK repository
├── src/          # Source code directory
│   ├── action/       # Loader and compiler wrappers
│   ├── cli/          # Command-line tools
│   ├── common/       # Common code (types, enums, errors)
│   ├── compiler/     # Multi-pass compiler
│   ├── entrypoint/   # JIT function entry assembly
│   ├── host/         # WASI & test host APIs
│   ├── platform/     # SGX/POSIX environment wrappers
│   ├── runtime/      # Runtime library
│   ├── singlepass/   # Single-pass compiler
│   ├── tests/        # Specification tests
│   ├── utils/        # Utility functions
│   └── wni/          # Host API module declarations
├── tests/        # Basic test cases
├── third_party/  # External dependencies
└── tools/        # Static code analysis tools
```

- `src/action`: Modules related to parsing/instantiation/compilation, including:
   - `module_loader`, `function_loader`: Responsible for parsing and validating wasm modules from files or byte streams
   - `instantiator`: Responsible for instantiating wasm modules
   - `interpreter`: Code related to the interpreter
   - `bytecode_visitor`: Parses wasm functions during JIT compilation
   - `compiler`: Wrapper code for JIT compilation
- `src/cli`: Code related to command-line tools
- `src/common`: Important data structures, including:
   - `wasm_defs`: Definitions related to wasm bytecode (mainly opcodes)
   - `const_string_pool`: Code related to constant pools/symbols
   - `error`: Data structures for error handling
   - `mem_pool`: Memory pool
   - `thread_pool`: Thread pool
   - `traphandler`: Hardware exceptions and signals
   - `type`: Wasm types
- `src/entrypoint`: JIT code execution entry point
- `src/host`: Host module
- `src/platform`: Functions related to the operating system
- `src/runtime`: Core runtime part of the virtual machine, including:
   - `src/runtime/codeholder`: Storage management for wasm bytecode
   - `src/runtime/instance`: Wasm runtime instance
   - `src/runtime/module`: Wasm module
   - `src/runtime/isolation`: Isolation for wasm runtime instances
   - `src/runtime/memory`: mmap pool mechanism
   - `src/runtime/runtime`: Core driver module of the virtual machine
- `src/singlepass`: Single-pass JIT compiler
- `src/tests`: Specification test framework
- `src/utils`: Utility structures

# 5. Auxiliary Features

### Error Handling
ZetaEngine places all possible error codes in the `src/common/errors.def` file. To throw an error during development, you can write the following code:
```cpp
if (...) {
  throw getError(ErrorCode::XXXXXXX)
}
```
> - XXXXXXX represents the error code you intend to throw.
> - Note the namespace: both `getError` and `ErrorCode` are members of `namespace zen::common`.

Use `getErrorWithPhase` or `getErrorWithExtraMessage` instead of `getError` for more fine-grained custom error throwing.

### Logging
ZetaEngine uses [spdlog](https://github.com/gabime/spdlog) as its logging library. To use logging, you need to:

1. Enable the `ZEN_ENABLE_LOGGING` macro during compilation.
2. Call the `zen::setGlobalLogger` function, passing in a `spdlog::logger` instance you create (a `spdlog::logger` for stdout is already created in the command-line interface program; you only need to specify the `--log-level` parameter when executing the CLI program).
3. Include `#include "utils/logging.h"` and call functions like `ZEN_LOG_TRACE`, `ZEN_LOG_DEBUG`, `ZEN_LOG_INFO`, `ZEN_LOG_WARN`, `ZEN_LOG_ERROR`, `ZEN_LOG_FATAL`.

### Memory Pool
In many places, you will see variables declared as `MemPool MPool`, and these variables are used for memory allocation and deallocation via `MPool->allocate` and `MPool->deallocate`. This is ZetaEngine's memory pool mechanism, designed to prevent memory leaks. It is strongly recommended to use this mechanism everywhere to reduce the likelihood of memory leaks.

### String Constant Pool
ZetaEngine uses a string constant pool mechanism to manage strings when processing Wasm files, improving performance in scenarios with a large number of strings. The steps to use this mechanism are as follows:

1. Call the `newSymbol` function with the raw string pointer and length as parameters to obtain an integer Symbol for the string.
2. To compare strings, directly compare their corresponding Symbols.
3. Call `dumpSymbolString` to retrieve the original string for a Symbol.
4. If the string is not a built-in constant string of the virtual machine (see the list in `src/common/const_strings.def`), call `freeSymbol` to release the string; otherwise, it will cause a memory leak.

# 6. Developing Host Modules
ZetaEngine allows you to develop your own Host modules. You can either create a new subdirectory under the `src/host` directory in the ZetaEngine project or develop the Host module in your own project and link it with ZetaEngine. The following steps illustrate the second approach:

1. Create a header file and a `cpp` file, e.g., `foo.h` and `foo.cpp`.
2. Write the following code in the header file:
```cpp
#pragma once

#include "zetaengine/zetaengine.h" // Adjust the path based on your project's build configuration

namespace foo_ns {
#undef EXPORT_MODULE_NAME
#define EXPORT_MODULE_NAME foo_module // Replace foo_module with the desired Host module name

AUTO_GENERATED_FUNCS_DECL
} // namespace foo_ns
```

3. Write the following code in the `cpp` file:
```cpp
#include "foo.h"

namespace foo_ns {

static void *vnmi_init_ctx(VNMIEnv *vmenv, const char *dir_list[],
                           uint32_t dir_count, const char *envs[],
                           uint32_t env_count, char *env_buf,
                           uint32_t env_buf_size, char *argv[], uint32_t argc,
                           char *argv_buf, uint32_t argv_buf_size) {
  return nullptr;
}

static void vnmi_destroy_ctx(VNMIEnv *vmenv, void *ctx) {}

// The first parameter of the function must be of type `zen::runtime::Instance *`
static int add(zen::runtime::Instance *Inst, int a, int b) {
  return a + b;
}

static int sub(zen::runtime::Instance *Inst, int a, int b) {
  return a - b;
}

#define FUNCTION_LISTS     \
    NATIVE_FUNC_ENTRY(add) \
    NATIVE_FUNC_ENTRY(sub)

#include "zetaengine/wni/boilerplate.cpp"
} // namespace foo_ns
```

4. Write the following code where you start the ZetaEngine runtime to load the Host module:
```cpp
#include "foo.h"

// ...

auto RT = zen::runtime::Runtime::newRuntime();
if (!RT) {
  // throw an error as needed
}

if (!LOAD_HOST_MODULE(RT, foo_ns, foo_module)) {
  // throw an error as needed
}

// Load and invoke the wasm file (omitted)

// ...
```
> Currently, only Host Functions are supported; Host Memory/Host Table/Host Global are not supported.

5. To merge multiple Host modules, first load the main Host module, then merge other Host modules into the main Host module as shown below. If there are modules or functions with the same name, the merge will fail:
```cpp
#include "foo.h"

// ...

auto RT = zen::runtime::Runtime::newRuntime();
if (!RT) {
  // throw an error as needed
}

zen::runtime::HostModule *HostMod = LOAD_HOST_MODULE(RT, foo_ns, foo_module);
if (!HostMod) {
  // throw an error as needed
}

if (!MERGE_HOST_MODULE(RT, *HostMod, foo_ns, foo_other_module)) {
  // throw an error as needed
}
```
> The actual effect of merging Host modules is equivalent to mounting the functions of other Host modules under the main Host module.

6. ZetaEngine also provides a function filtering feature for Host modules. Simply call the `filterFunctions` method of the `HostModule` class:
```cpp
#include "foo.h"

// ...

auto RT = zen::runtime::Runtime::newRuntime();
if (!RT) {
  // throw an error as needed
}

zen::runtime::HostModule *HostMod = LOAD_HOST_MODULE(RT, foo_ns, foo_module);
if (!HostMod) {
  // throw an error as needed
}
if (!HostMod->filterFunctions({"func_1", "func_2"})) {
  // throw an error as needed
}
```
> If the Host functions in the whitelist do not exist, the `filterFunctions` method will return an error, which you can handle as needed.

# 7. Coverage Testing

1. Add the `-DZEN_ENABLE_COVERAGE=ON` parameter during CMake build configuration.
2. Run all test cases.
3. Use `lcov` and `genhtml` to generate a coverage report.

The specific commands are as follows:
```shell
# Clone the repository
...

# CMake configuration & build
cmake -S . -B build -DZEN_ENABLE_SINGLEPASS_JIT=ON \
                    -DZEN_ENABLE_SPEC_TEST=ON \
                    -DZEN_ENABLE_COVERAGE=ON
cmake --build build -j 

# Run spec tests in the project directory
cd build && ./specUnitTest 1

# Collect coverage information files generated after running tests
lcov -c -d . -o coverage.info

# Exclude irrelevant paths
lcov --remove coverage.info '/usr/include/' '*/build/_deps/*' '*/src/common/wasm_defs/*' -o coverage.info

# Generate an HTML report
genhtml coverage.info -o COVERAGE

# Start a server to view the HTML report (12345 is the port, can be changed as needed)
python3 -m http.server 12345 --directory COVERAGE
```