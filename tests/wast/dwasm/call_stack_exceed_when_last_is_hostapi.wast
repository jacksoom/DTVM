;; testcase for exceeded max call stack(1025), when last call frame is hostapi call

(module
  (type (func (param i32)))
  (func $print_i32 (import "spectest" "print_i32") (type 0))
  (func $func2 (param i32) (param i32) (result i32)
    (if (result i32) (i32.eqz (local.get 0))
        (then (block (result i32)
                   (call $print_i32 (i32.const 1))
                   (local.get 1)))
        (else (call $func2
                (i32.add (local.get 0) (i32.const -1))
                (i32.add (local.get 1) (i32.const 1))
         ))
    ))
  (func (export "test2out") (result i32)
        (call $func2 (i32.const 80000)
                     (i32.const 0)))
)

(assert_trap (invoke "test2out") "error_code: 90001\nerror_msg: WasmCallStackExceed")
