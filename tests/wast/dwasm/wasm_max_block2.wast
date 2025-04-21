;; testcase for max block(children blocks= 1024)

(module
  (func (export "test") (result i32)
        (local i32)
        (local.set 0 (i32.const 0))
        (block
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))
            (block (drop 
                (i32.add (local.get 0) (i32.const 1))))

             )
        (i32.const 1))
)

(assert_return (invoke "test") (i32.const 1))
