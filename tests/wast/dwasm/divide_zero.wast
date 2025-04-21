;; test case of divide zero

(module
  (func (export "test") (result i32) (i32.div_s (i32.const 1) (i32.const 0)))
)

(assert_trap (invoke "test") "error_code: 90003\nerror_msg: integer divide by zero")

(module
  (func (export "div_s") (param $x i32) (param $y i32) (result i32) (i32.div_s (local.get $x) (local.get $y)))
  (func (export "div_u") (param $x i32) (param $y i32) (result i32) (i32.div_u (local.get $x) (local.get $y)))
  (func (export "rem_s") (param $x i32) (param $y i32) (result i32) (i32.rem_s (local.get $x) (local.get $y)))
  (func (export "rem_u") (param $x i32) (param $y i32) (result i32) (i32.rem_u (local.get $x) (local.get $y))) 
)

(assert_trap (invoke "div_s" (i32.const 1) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")
(assert_trap (invoke "div_s" (i32.const 0) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")
(assert_trap (invoke "div_s" (i32.const 0x80000000) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")

(assert_trap (invoke "div_u" (i32.const 1) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")
(assert_trap (invoke "div_u" (i32.const 0) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")

(assert_trap (invoke "rem_s" (i32.const 1) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")
(assert_trap (invoke "rem_s" (i32.const 0) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")

(assert_trap (invoke "rem_u" (i32.const 1) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")
(assert_trap (invoke "rem_u" (i32.const 0) (i32.const 0)) "error_code: 90003\nerror_msg: integer divide by zero")