(module
  (func (export "test_i32_wrap_i64") (result i32)
    (i32.load (i32.wrap_i64 (i64.const 4294967297)))
    )

  (memory 1)
  (data (i32.const 0) "")
)

(assert_return (invoke "test_i32_wrap_i64") (i32.const 0))
