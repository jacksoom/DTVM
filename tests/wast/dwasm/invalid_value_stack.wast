;; testcase for invalid value stack

(assert_invalid
(module
  (func (export "test") (result i32)
        (i32.add (i32.const 1) (i32.const 0) (i32.const 0)))
)
 "error_code: 10097\nerror_msg: WasmModuleFormatInvalid")
