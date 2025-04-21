;; testcase of hostapi call wasm(which disabled in dwasm)

(module
  (type (func (param i32)))
  (func $call_wasm (import "spectest" "call_wasm") (type 0))
  (func $func1 (param i32) (result i32)
    (i32.const 123))
  (func $func2 (param i32) (result i32)
    (call $call_wasm (i32.const 1))
    (i32.const 123)
    )
  (func (export "test") (result i32)
        (call $func2 (i32.const 1023)
                     ))
)

;; hostapi call wasm will return empty string error now
(assert_trap (invoke "test") "error_code: 90100\nerror_msg: InvalidHostApiCallWasm")
