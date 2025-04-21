(module
  (func $func0 (param i32) (result i32)
    (i32.div_s (i32.const 1) (local.get 0))
  )
  (func (export "run")
    (drop (call $func0 (i32.const 0)))
    (unreachable)
  )
)

(assert_trap (invoke "run") "integer divide by zero")
