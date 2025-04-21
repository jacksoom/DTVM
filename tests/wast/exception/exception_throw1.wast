(module
  (func (export "run")
    (drop (i32.div_s (i32.const 2) (i32.const 0)))
    (unreachable)
  )
)

(assert_trap (invoke "run") "integer divide by zero")
