;; testcase for invalid call target

(assert_invalid
(module
  (func (export "test") (result i32)
        (call 1)
        (i32.const 1))
)
 "error_code: 10097\nerror_msg: WasmModuleFormatInvalid")
