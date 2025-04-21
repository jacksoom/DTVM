/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include "Enclave_t.h"
#include "zetaengine-c.h"

int ecall_printf(const char *fmt, ...) 
{
  char buf[BUFSIZ] = {'\0'};
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, BUFSIZ, fmt, ap);
  va_end(ap);
  ocall_print_string(buf);
  return (int)strnlen(fmt, BUFSIZ - 1) + 1;
}


/* spec test begin */
static void print(ZenInstanceRef instance) {}

static void print_i32(ZenInstanceRef instance, int32_t i32) {
  ecall_printf("0x%x:i32\n", i32);
}

static void print_i32_f32(ZenInstanceRef instance, int32_t i32, float f32) {
  ecall_printf("0x%x:i32\n%.7g:f32\n", i32, f32);
}

static void print_f64_f64(ZenInstanceRef instance, double f64_1, double f64_2) {
  ecall_printf("%.7g:f64\n%.7g:f64\n", f64_1, f64_2);
}

static void print_f32(ZenInstanceRef instance, float f32) {
  ecall_printf("%.7g:f32\n", f32);
}

static void print_f64(ZenInstanceRef instance, double f64) {
  ecall_printf("%.7g:f64\n", f64);
}
/* spec test end */

static char **
ecall_split_string(char *str, int *count)
{
  char **res = NULL;
  char *p;
  int idx = 0;

  /* split string and append tokens to 'res' */
  do {
      p = strtok(str, " ");
      str = NULL;
      res = (char **)realloc(res, sizeof(char *) * (unsigned)(idx + 1));
      if (res == NULL) {
          return NULL;
      }
      res[idx++] = p;
  } while (p);

  /**
    * since the function name,
    * res[0] might be contains a '\' to indicate a space
    * func\name -> func name
    */
  p = strchr(res[0], '\\');
  while (p) {
      *p = ' ';
      p = strchr(p, '\\');
  }

  if (count) {
      *count = idx - 1;
  }
  return res;
}

void ecall_print_result(ZenValue Results[], uint32_t *NumResults) {
  if (*NumResults < 1) {
    ocall_print_string("\n");
    return;
  }

  for (uint32_t I = 0; I < *NumResults; I++) {
    ZenValue Result = Results[I];
    switch (Result.Type) {
      case ZenTypeI32:
      ecall_printf("0x%" PRIx32 ":i32\n", Result.Value.I32);
      break;
      case ZenTypeI64:
      ecall_printf("0x%" PRIx64 ":i64\n", Result.Value.I64);
      break;
      case ZenTypeF32:
      ecall_printf("%.7g:f32\n", Result.Value.F32);
      break;
      case ZenTypeF64:
      ecall_printf("%.7g:f64\n", Result.Value.F64);
      break;
    }
    if (I < (*NumResults - 1)) {
      ocall_print_string(",");
    }
  }
}

void ecall_print_error(ZenInstanceRef Instance, const char *FuncName) {
  char ErrBuf[1024];
  uint32_t ErrBufSize = 1024;
  if (!ZenGetInstanceError(Instance, static_cast<char *>(ErrBuf), ErrBufSize)) {
    ecall_printf("Failed to get error info from instance.\n");
  }
  ecall_printf("Failed to call function '%s': %s\n", FuncName, ErrBuf);
  ZenClearInstanceError(Instance);
}

int ecall_wasm_repl(ZenRuntimeRef Runtime, ZenInstanceRef Instance, ZenValue Results[],
                           uint32_t *NumResults) {
  char cmd[1024];
  size_t len = 0;
  int n = 0;
  int app_argc;
  char **app_argv;
    
  while(ecall_printf("webassembly> "), ocall_getline(static_cast<char *>(cmd), 1024, &n, &len), n != -1) {
    assert(n > 0);
    if (cmd[n - 1] == '\n') {
        if (n == 1)
            continue;
        else
            cmd[n - 1] = '\0';
    }
    if (!strcmp(cmd, "__exit__")) {
        ecall_printf("exit repl mode\n");
        break;
    }
    app_argv = ecall_split_string(cmd, &app_argc);
    if (app_argv == NULL) {
        ecall_printf("Wasm prepare param failed: split string failed.\n");
        break;
    }
    if (app_argc != 0) {
        if (!ZenCallWasmFuncByName(Runtime, Instance, app_argv[0], (const char**)(app_argv + 1), app_argc - 1, Results,
                          NumResults)) {
          ecall_printf("func name : %s, args 0 : %s \n", app_argv[0], app_argv[1]);
          ecall_print_error(Instance, app_argv[0]);
        } else {
          ecall_print_result(Results, NumResults);
        }
    }
    free(app_argv);
  }
  return 0;
}

int ecall_wasm_func(const char *wasm_name, const char *func_name, const char *args[], uint32_t args_num, const char *wasm_file_buf, size_t wasm_file_size, ZenRunMode mode, bool is_repl_mode) {
  ZenValue Results[1] = {(ZenType)0};
  uint32_t NumResults;

  /// ================ Create ZetaEngine runtime ================

  ZenRuntimeConfigRef config = ZenCreateRuntimeConfig(mode);
  ZenRuntimeConfigSetWasmMemoryMap(config, false);

  auto Runtime = ZenCreateRuntime(config);
  if (!Runtime) {
    ecall_printf("create runtime failed\n");
    return -1;
  }

  /// ================ Load spectest module ================

  ZenType ArgTypesI32[] = {ZenTypeI32};
  ZenType ArgTypesF32[] = {ZenTypeF32};
  ZenType ArgTypesF64[] = {ZenTypeF64};
  ZenType ArgTypesI32F32[] = {ZenTypeI32, ZenTypeF32};
  ZenType ArgTypesF64F64[] = {ZenTypeF64, ZenTypeF64};
  ZenHostFuncDesc HostFuncDescs[] = {
    {
      .Name = "print",
      .NumArgs = 0,
      .ArgTypes = NULL,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print,
    },
    {
      .Name = "print_i32",
      .NumArgs = 1,
      .ArgTypes = ArgTypesI32,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print_i32,
    },
    {
      .Name = "print_i32_f32",
      .NumArgs = 2,
      .ArgTypes = ArgTypesI32F32,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print_i32_f32,
    },
    {
      .Name = "print_f64_f64",
      .NumArgs = 2,
      .ArgTypes = ArgTypesF64F64,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print_f64_f64,
    },
    {
      .Name = "print_f32",
      .NumArgs = 1,
      .ArgTypes = ArgTypesF32,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print_f32,
    },
    {
      .Name = "print_f64",
      .NumArgs = 1,
      .ArgTypes = ArgTypesF64,
      .NumReturns = 0,
      .RetTypes = NULL,
      .Ptr = (void *)print_f64,
    }
  };
  ZenHostModuleDescRef SpecModDesc = ZenCreateHostModuleDesc(Runtime, "spectest", HostFuncDescs, 6);
  ZenHostModuleRef SpecMod = ZenLoadHostModule(Runtime, SpecModDesc);

  /// ================ Load user's module ================

  char ErrBuf[256];
  uint32_t ErrBufSize = 256;
  std::vector<unsigned char> wasm_bytecode(wasm_file_buf, wasm_file_buf + wasm_file_size);
  auto Module =
      ZenLoadModuleFromBuffer(Runtime, wasm_name, wasm_bytecode.data(),
                              wasm_bytecode.size(), ErrBuf, ErrBufSize);
  if (!Module) {
    ZenDeleteHostModule(Runtime, SpecMod);
    ZenDeleteHostModuleDesc(Runtime, SpecModDesc);
    ZenDeleteRuntime(Runtime);
    ecall_printf("load module failed: %s\n", ErrBuf);
    return -2;
  }

  /// ================ Create isolation ================

  auto Isolation = ZenCreateIsolation(Runtime);

  if (!Isolation) {
    ZenDeleteModule(Runtime, Module);
    ZenDeleteHostModule(Runtime, SpecMod);
    ZenDeleteHostModuleDesc(Runtime, SpecModDesc);
    ZenDeleteRuntime(Runtime);
    ecall_printf("create isolation failed!\n");
    return -3;
  }

  /// ================ Create instance ================

  auto Instance = ZenCreateInstance(Isolation, Module, ErrBuf, ErrBufSize);

  if (!Instance) {
    ZenDeleteModule(Runtime, Module);
    ZenDeleteHostModule(Runtime, SpecMod);
    ZenDeleteHostModuleDesc(Runtime, SpecModDesc);
    ZenDeleteIsolation(Runtime, Isolation);
    ZenDeleteRuntime(Runtime);
    ecall_printf("create instance failed: %s\n", ErrBuf);
    return -4;
  }

  /// ================ Call function ================

  if (is_repl_mode) {
    ecall_wasm_repl(Runtime, Instance, Results,
                             &NumResults);
  } else if (func_name) {
    if(!ZenCallWasmFuncByName(Runtime, Instance, func_name, args, args_num, Results,
                             &NumResults)){
      ecall_print_error(Instance, func_name);
    } else {
      ecall_print_result(Results, &NumResults);
    }
  } else {
    if(!ZenCallWasmMainFunc(Runtime, Instance, Results, &NumResults)){
      ecall_print_error(Instance, "main");
    } else {
      ecall_print_result(Results, &NumResults);
    }
  }

  /// ================ Release resuorce ================

  ZenDeleteInstance(Isolation, Instance);
  ZenDeleteIsolation(Runtime, Isolation);
  ZenDeleteModule(Runtime, Module);
  ZenDeleteHostModule(Runtime, SpecMod);
  ZenDeleteHostModuleDesc(Runtime, SpecModDesc);
  ZenDeleteRuntime(Runtime);
  ZenDeleteRuntimeConfig(config);
  return 0;
}

int ecall_main(int argc, void *argv) {
  char **params = (char **)argv;
  const char *func_name = NULL;
  const char *args[10];
  ZenRunMode mode = ZenModeInterp;
  uint32_t args_num = 0;
  std::string path(params[1]);
  std::size_t found = path.find_last_of("/");
  std::string filename = path.substr(found + 1);
  const char * wasm_file_name = filename.c_str();
  const char * wasm_file_buf = params[argc - 2];
  size_t wasm_file_size = strtoul(params[argc - 1], NULL, 10);
  bool is_repl_mode = false;
  argc-=2;
  if (argc < 1) {
    ecall_printf("wasm params error\n");
    return -2;
  }

  /* Process options. */
  for (int i = 0; i < argc; i++) {
    char *param = params[i];
    if (!strcmp(param, "-f") || !strcmp(param, "--function")) {
      func_name = params[++i];
      while (i++ && i < argc) {
        args[args_num++] = params[i];
      }
    } 
    else if (!strcmp(param, "-m") || !strcmp(param, "--mode")) {
      char *mode_name = params[++i];
      if (!strcmp(mode_name, "1") || !strcmp(mode_name, "singlepass")) {
        mode = ZenModeSinglepass;
      }
    } 
    else if (!strcmp(param, "--repl")) {
      is_repl_mode = true;
    }
  }

  ecall_wasm_func(wasm_file_name, func_name, args, args_num, wasm_file_buf, wasm_file_size, mode, is_repl_mode);
  return 0;
}

