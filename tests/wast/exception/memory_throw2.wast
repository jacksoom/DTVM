(module
  (memory 1)
      (func (export "64_bad") (param $i i32)
          (drop (f64.load offset=4294967295 (local.get $i)))
        )

    (func (export "64_bad2") (param $i i32)
              (drop (f64.load offset=2147483648 (local.get $i)))
            )

    (func (export "64_bad3") (param $i i32)
              (drop (f64.load offset=2147483647 (local.get $i)))
            )

  (data (i32.const 0) "a")
  (data (i32.const 1000) "ffffffff")
)

(assert_trap (invoke "64_bad" (i32.const 0)) "out of bounds memory access")
(assert_trap (invoke "64_bad" (i32.const 1)) "out of bounds memory access")
(assert_trap (invoke "64_bad2" (i32.const 0)) "out of bounds memory access")
(assert_trap (invoke "64_bad3" (i32.const 2)) "out of bounds memory access")
