;; testcase for invalid numeric in wasm module(eg. elem index > table size, data segment index invalid)


(assert_invalid
(module
  (import "invalid-env" "invalid-hostapi" (func (param i32)))
  (func (export "test") (result i32)
        (i32.const 1))
)
 "error_code: 10090\nerror_msg: UnlinkedImportFunc")
