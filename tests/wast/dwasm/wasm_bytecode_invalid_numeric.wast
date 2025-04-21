;; testcase for invalid numeric in wasm module(eg. elem index > table size, data segment index invalid)

(assert_invalid
(module
  (table (;0;) 1 2 funcref)
  (func (;0;) (result i32)
      i32.const 123)
  (elem (;0;) (i32.const 3) func 0) ;; invalid index
  (func (export "test") (result i32)
        (i32.const 1))
)
 "error_code: 10097\nerror_msg: WasmModuleFormatInvalid")
