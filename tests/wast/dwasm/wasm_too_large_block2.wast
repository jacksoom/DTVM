;; testcase for max block(children blocks > 1024)

(assert_invalid
(module
  (func (export "test") (result i32)
        (local i32)
        (local.set 0 (i32.const 0))
        (block
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
                          (block
                              (drop (i32.add (local.get 0) (i32.const 1))))
             )
        (i32.const 1))
)
 "error_code: 10060\nerror_msg: WasmBlockTooLarge")
