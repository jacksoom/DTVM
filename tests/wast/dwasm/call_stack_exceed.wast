;; testcase for exceeded max call stack(1025). when last call frame is not hostapi

(module
  (func $func1 (param i32) (param i32) (result i32)
    (if (result i32) (i32.eqz (local.get 0))
        (then (local.get 1))
        (else (call $func1
                (i32.add (local.get 0) (i32.const -1))
                (i32.add (local.get 1) (i32.const 1))
         ))
    ))
  (func (export "test") (result i32)
        (call $func1 (i32.const 80000)
                     (i32.const 0)))
)

(assert_trap (invoke "test") "error_code: 90001\nerror_msg: WasmCallStackExceed")
