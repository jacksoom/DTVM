;; testcase for max call stack(1024)

(module
  (func $func1 (param i32) (param i32) (result i32)
    (if (result i32) (i32.eqz (local.get 0))
        (then (local.get 1))
        (else (call $func1
                (i32.add (local.get 0) (i32.const -1))
                (i32.add (local.get 1) (i32.const 1))
         ))
    ))
  (func (export "test") (result i32)
        (call $func1 (i32.const 1022)
                     (i32.const 0)))
)

(assert_return (invoke "test") (i32.const 1022))
