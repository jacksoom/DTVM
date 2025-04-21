;; testcase of too many table elements in wasm-module(10241 > 10240)

(assert_invalid
(module
  (func $f (result i32) (i32.const 123))
  (table 10241 10241 funcref)
  (elem (i32.const 10240) $f)
  (func (export "test") (result i32)
          (i32.const 1))
)
"error_code: 10093\nerror_msg: WasmModuleElementTooLarge")
