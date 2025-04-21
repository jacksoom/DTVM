(module
  (type (func (param i32)))
  (func $call_wasm (import "spectest" "call_wasm") (type 0))
  (func $func0
    (drop (i32.div_s (i32.const 2) (i32.const 0)))
  )
  (func (export "run")
    (call $call_wasm (i32.const 1))
    (unreachable)
  )
)

(assert_trap (invoke "run") "integer divide by zero")
