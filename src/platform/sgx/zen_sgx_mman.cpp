/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "platform/sgx/zen_sgx_mman.h"
#include "inttypes.h"
#include "platform/sgx/zen_sgx.h"
#include "sgx_error.h"
#include "sgx_rsrv_mem_mngr.h"
#include "unistd.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>

void *mmap(void *Addr, size_t Size, int Prot, int Flags, int Fd,
           size_t Offset) {
  int MProt = 0;
  uint64_t AlignedSize, PageSize;
  void *Ret = nullptr;
  sgx_status_t Status = SGX_SUCCESS;

  PageSize = getpagesize();
  AlignedSize = (Size + PageSize - 1) & ~(PageSize - 1);

  if (AlignedSize >= UINT32_MAX)
    return nullptr;

  Ret = sgx_alloc_rsrv_mem(AlignedSize);

  if (Ret == nullptr) {
    printf("os_mmap(size=%u, aligned size=%lu, prot=0x%x) failed.", Size,
           AlignedSize, Prot);
    return nullptr;
  }

  if (Prot & PROT_READ)
    MProt |= SGX_PROT_READ;
  if (Prot & PROT_WRITE)
    MProt |= SGX_PROT_WRITE;
  if (Prot & PROT_EXEC)
    MProt |= SGX_PROT_EXEC;
  if (Prot & PROT_NONE)
    MProt |= SGX_PROT_NONE;

  Status = sgx_tprotect_rsrv_mem(Ret, AlignedSize, MProt);
  if (Status != SGX_SUCCESS) {
    printf("os_mmap(size=%u, prot=0x%x) failed to set protect.\n", Size, Prot);
    sgx_free_rsrv_mem(Ret, AlignedSize);
    return nullptr;
  }

  return Ret;
}

int munmap(void *Addr, size_t Size) {
  uint64_t AlignedSize, PageSize;
  PageSize = getpagesize();
  AlignedSize = (Size + PageSize - 1) & ~(PageSize - 1);
  sgx_free_rsrv_mem(Addr, AlignedSize);
}

int mprotect(void *Addr, size_t Size, int Prot) {
  int MProt = 0;
  sgx_status_t Status = SGX_SUCCESS;
  uint64_t AlignedSize, PageSize;

  PageSize = getpagesize();
  AlignedSize = (Size + PageSize - 1) & ~(PageSize - 1);

  if (Prot & PROT_READ)
    MProt |= SGX_PROT_READ;
  if (Prot & PROT_WRITE)
    MProt |= SGX_PROT_WRITE;
  if (Prot & PROT_EXEC)
    MProt |= SGX_PROT_EXEC;
  Status = sgx_tprotect_rsrv_mem(Addr, AlignedSize, MProt);
  if (Status != SGX_SUCCESS)
    printf("os_mprotect(addr=0x%" PRIx64 ", size=%u, prot=0x%x) failed.",
           (uintptr_t)Addr, Size, Prot);

  return (Status == SGX_SUCCESS ? 0 : -1);
}