#!/bin/bash
cd example
wast2json fib.wast
wast2json demo_hostapi.wast
cd ..
# if you change the dtvm lib, use ./build.sh to clean cache
cargo test -- --nocapture
