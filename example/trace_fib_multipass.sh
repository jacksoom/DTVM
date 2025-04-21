#!/bin/bash
export GREEDY_FUNC_IDX_START=0
export GREEDY_FUNC_IDX_END=100000
./tools/gdb_trace.py ./build/dtvm --mode 2 --enable-gdb-tracing-hook -f fib ./example/fib.0.wasm --args 10 multipass.log
