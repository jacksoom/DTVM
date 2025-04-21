;; case test gas. spectest use 10000 as init gas

(module
  (func (export "test")
    (result i32)
      (call $__instrumented_use_gas (i64.const 123))
     (i32.div_s (i32.const 1) (i32.const 1)))
  ;; treaky export func to call func but get gas
  (func (export "test$gas") (result i64) (i64.const 0))
  (func $__instrumented_use_gas (export "__instrumented_use_gas") (param i64))
)
(assert_return (invoke "test") (i32.const 1))
(assert_return (invoke "test$gas") (i64.const 9877)) ;; xxx$gas means call test but only return gas

;; case test gas after trap. need return remaining gas(if use gas register, need save gas from register to memory)

(module
  (func (export "test")
    (result i32)
      (call $__instrumented_use_gas (i64.const 1))
     (i32.div_s (i32.const 1) (i32.const 0))) ;; divide by zero trap
   ;; treaky export func to call func but get gas
   (func (export "test$gas") (result i64) (i64.const 0))
  (func $__instrumented_use_gas (export "__instrumented_use_gas") (param i64))
)

;; test gas after trap
(assert_trap (invoke "test") "integer divide by zero")
(assert_trap (invoke "test$gas") "9999")

;; case that test out of gas
(module
  (func (export "test")
    (result i32)
      (call $__instrumented_use_gas (i64.const 1))
      (call $__instrumented_use_gas (i64.const 10000)) ;; gas greater than spectest init 10000 gas
     (i32.div_s (i32.const 1) (i32.const 1)))
   ;; treaky export func to call func but get gas
   (func (export "test$gas") (result i64) (i64.const 0))
  (func $__instrumented_use_gas (export "__instrumented_use_gas") (param i64))
)
(assert_trap (invoke "test") "out of gas")
(assert_trap (invoke "test$gas") "0")
