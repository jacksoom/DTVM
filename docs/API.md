# ZetaEngine API Documentation

ZetaEngine provides three layers of APIs with different levels of flexibility and use cases, supporting C, C++ and SGX environments.

## 1. Standalone Runtime API (DefaultRuntime)

The simplest API layer designed for standalone VM usage. This is the least flexible but most straightforward API group.

### C++ API Usage
```cpp
#include "zetaengine.h"

// Set up global logger
zen::setGlobalLogger(createConsoleLogger("app_logger", LoggerLevel::Info));

// Create runtime with configuration
RuntimeConfig config;
config.Mode = RunMode::SinglepassMode;

auto runtime = Runtime::newRuntime(config);

// Load module
auto module = runtime->loadModule("module.wasm");

// Create isolation
auto isolation = runtime->createManagedIsolation();

// Create instance
auto instance = isolation->createInstance(*module);

// Call function
std::vector<TypedValue> results;
runtime->callWasmFunction(*instance, "func_name", args, results);

// Cleanup
isolation->deleteInstance(instance);
runtime->deleteManagedIsolation(isolation);
runtime->unloadModule(module);
```

### C API Usage
```c
#include "zetaengine-c.h"

// Create runtime configuration
ZenRuntimeConfig config = {
    .Mode = ZenModeSinglepass,
    .DisableWasmMemoryMap = false,
    .DisableWASI = true,
    .EnableStatistics = false,
    .EnableGdbTracingHook = false
};

// Create runtime
ZenRuntimeRef runtime = ZenCreateRuntime(&config);

// Load module
char err_buf[128];
ZenModuleRef module = ZenLoadModuleFromFile(runtime, "module.wasm", err_buf, sizeof(err_buf));

// Create isolation and instance
ZenIsolationRef isolation = ZenCreateIsolation(runtime);
ZenInstanceRef instance = ZenCreateInstance(isolation, module, err_buf, sizeof(err_buf));

// Call function
ZenValue results[1];
uint32_t num_results;
ZenCallWasmFuncByName(runtime, instance, "func_name", NULL, 0, results, &num_results);

// Cleanup
ZenDeleteInstance(isolation, instance);
ZenDeleteIsolation(runtime, isolation);
ZenDeleteModule(runtime, module);
ZenDeleteRuntime(runtime);
```

### SGX API Usage
```cpp
// In Enclave.cpp
#include "zetaengine-c.h"

// Create runtime configuration
ZenRuntimeConfig config = {
    .Mode = ZenModeSinglepass,
    .DisableWasmMemoryMap = false,
    .DisableWASI = true,
    .EnableStatistics = false,
    .EnableGdbTracingHook = false
};

// Create runtime
ZenRuntimeRef runtime = ZenCreateRuntime(&config);

// Load host module (spectest)
ZenType ArgTypesI32[] = {ZenTypeI32};
ZenHostFuncDesc HostFuncDescs[] = {
    {
        .Name = "print_i32",
        .NumArgs = 1,
        .ArgTypes = ArgTypesI32,
        .NumReturns = 0,
        .RetTypes = NULL,
        .Ptr = (void*)print_i32
    }
};
ZenHostModuleDescRef SpecModDesc = ZenCreateHostModuleDesc(runtime, "spectest", HostFuncDescs, 1);
ZenHostModuleRef SpecMod = ZenLoadHostModule(runtime, SpecModDesc);

// Load user module
char ErrBuf[256];
uint32_t ErrBufSize = 256;
ZenModuleRef Module = ZenLoadModuleFromBuffer(runtime, "module_name", wasm_bytecode.data(), 
                                             wasm_bytecode.size(), ErrBuf, ErrBufSize);

// Create isolation and instance
ZenIsolationRef Isolation = ZenCreateIsolation(runtime);
ZenInstanceRef Instance = ZenCreateInstance(Isolation, Module, ErrBuf, ErrBufSize);

// Call function
ZenValue Results[1];
uint32_t NumResults;
ZenCallWasmFuncByName(runtime, Instance, func_name, args, args_num, Results, &NumResults);

// Cleanup
ZenDeleteInstance(Isolation, Instance);
ZenDeleteIsolation(runtime, Isolation);
ZenDeleteModule(runtime, Module);
ZenDeleteHostModule(runtime, SpecMod);
ZenDeleteHostModuleDesc(runtime, SpecModDesc);
ZenDeleteRuntime(runtime);
```

## 2. WebAssembly API

### C++ API Components

#### 1. Runtime Management
```cpp
// Create runtime with configuration
RuntimeConfig config;
config.Mode = RunMode::SinglepassMode;
auto runtime = Runtime::newRuntime(config);

// Configure WASI
runtime->setWASIArgs(wasm_filename, args);
runtime->setWASIEnvs(envs);
runtime->setWASIDirs(dirs);
```

#### 2. Module Management
```cpp
// Load module with entry hint
auto module = runtime->loadModule(wasm_filename, entry_hint);

// Load host module
HostModule* wasi_mod = LOAD_HOST_MODULE(runtime, zen::host, wasi_snapshot_preview1);
```

#### 3. Instance Management
```cpp
// Create instance with gas limit
auto instance = isolation->createInstance(*module, gas_limit);

// Call function
std::vector<TypedValue> results;
runtime->callWasmFunction(*instance, func_name, args, results);
```

### C API Components

#### 1. Runtime Configuration
```c
ZenRuntimeConfig config = {
    .Mode = ZenModeSinglepass,
    .DisableWasmMemoryMap = false,
    .DisableWASI = true,
    .EnableStatistics = false,
    .EnableGdbTracingHook = false
};
```

#### 2. Host Module Support
```c
ZenHostFuncDesc host_funcs[] = {
    {
        .Name = "print_i32",
        .NumArgs = 1,
        .ArgTypes = {ZenTypeI32},
        .NumReturns = 0,
        .RetTypes = NULL,
        .Ptr = (void*)print_i32
    }
};
ZenHostModuleDescRef host_desc = ZenCreateHostModuleDesc(runtime, "env", host_funcs, 1);
```

#### 3. Memory Management
```c
// Validate memory addresses
bool valid = ZenValidateHostMemAddr(instance, host_addr, size);
void* host_addr = ZenGetHostMemAddr(instance, offset);
```

### SGX Specific Features

#### 1. Secure Memory Management
```cpp
// Validate memory access
bool valid = ZenValidateHostMemAddr(instance, host_addr, size);
if (!valid) {
    // Handle invalid memory access
    return;
}
```

#### 2. Error Handling
```cpp
// Check for errors
char err_buf[256];
if (ZenGetInstanceError(instance, err_buf, sizeof(err_buf))) {
    // Handle error securely
    return;
}
```

#### 3. Gas Management
```cpp
// Set gas limit for secure execution
ZenInstanceRef instance = ZenCreateInstanceWithGas(isolation, module, gas_limit, err_buf, sizeof(err_buf));
```

## Best Practices

### C++ Environment
1. Use RAII for resource management
2. Leverage type safety features
3. Use appropriate logging levels
4. Handle exceptions properly

### C Environment
1. Always check error states
2. Use appropriate buffer sizes
3. Clean up resources properly
4. Validate memory access

### SGX Environment
1. Validate all memory access
2. Use secure error handling
3. Monitor gas usage
4. Implement proper cleanup
5. Use appropriate isolation

## Notes

- C++ API provides better type safety and resource management
- C API is more suitable for FFI and system integration
- SGX environment requires additional security considerations
- Memory management is critical in all environments
- Error handling varies by environment
- Performance characteristics may differ between environments
