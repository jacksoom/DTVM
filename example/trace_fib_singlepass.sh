#!/bin/bash
./tools/gdb_trace.py ./build/dtvm --mode 1 --enable-gdb-tracing-hook -f fib ./example/fib.0.wasm --args 10 singlepass.log
