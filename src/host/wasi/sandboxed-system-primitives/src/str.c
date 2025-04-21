// Part of the Wasmtime Project, under the Apache License v2.0 with LLVM
// Exceptions. See
// https://github.com/bytecodealliance/wasmtime/blob/main/LICENSE for license
// information.
//
// Significant parts of this file are derived from cloudabi-utils. See
// https://github.com/bytecodealliance/wasmtime/blob/main/lib/wasi/sandboxed-system-primitives/src/LICENSE
// for license information.
//
// The upstream file contains the following copyright notice:
//
// Copyright (c) 2016 Nuxi, https://nuxi.nl/

#include "str.h"
#include "ssp_config.h"
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

static char *bh_strndup(const char *s, size_t n) {
  size_t l = strnlen(s, n);
  char *s1 = malloc((uint32_t)(l + 1));

  if (!s1)
    return NULL;
  // bh_memcpy_s(s1, (uint32_t)(l + 1), s, (uint32_t)l);
  memcpy(s1, s, (uint32_t)l);
  s1[l] = 0;
  return s1;
}

char *str_nullterminate(const char *s, size_t len) {
  /* Copy string */
  char *ret = bh_strndup(s, len);

  if (ret == NULL)
    return NULL;

  /* Ensure that it contains no null bytes within */
  if (strlen(ret) != len) {
    free(ret);
    errno = EILSEQ;
    return NULL;
  }
  return ret;
}
