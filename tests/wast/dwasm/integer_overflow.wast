;; testcase of integer overflow


(module
  (func (export "test") (result i32) (i32.div_s (i32.const 0x80000000) (i32.const -1)))
)

(assert_trap (invoke "test") "error_code: 90002\nerror_msg: integer overflow")

(module
  (func (export "i32.trunc_f32_s") (param $x f32) (result i32) (i32.trunc_f32_s (local.get $x)))
  (func (export "i32.trunc_f32_u") (param $x f32) (result i32) (i32.trunc_f32_u (local.get $x)))
  (func (export "i32.trunc_f64_s") (param $x f64) (result i32) (i32.trunc_f64_s (local.get $x)))
  (func (export "i32.trunc_f64_u") (param $x f64) (result i32) (i32.trunc_f64_u (local.get $x)))
  (func (export "i64.trunc_f32_s") (param $x f32) (result i64) (i64.trunc_f32_s (local.get $x)))
  (func (export "i64.trunc_f32_u") (param $x f32) (result i64) (i64.trunc_f32_u (local.get $x)))
  (func (export "i64.trunc_f64_s") (param $x f64) (result i64) (i64.trunc_f64_s (local.get $x)))
  (func (export "i64.trunc_f64_u") (param $x f64) (result i64) (i64.trunc_f64_u (local.get $x)))
)

(assert_trap (invoke "i32.trunc_f32_s" (f32.const 2147483648.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_s" (f32.const -2147483904.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_s" (f32.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_s" (f32.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i32.trunc_f32_u" (f32.const 4294967296.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_u" (f32.const -1.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_u" (f32.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f32_u" (f32.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i32.trunc_f64_s" (f64.const 2147483648.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_s" (f64.const -2147483649.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_s" (f64.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_s" (f64.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i32.trunc_f64_u" (f64.const 4294967296.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const -1.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const 1e16)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const 1e30)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const 9223372036854775808)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i32.trunc_f64_u" (f64.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i64.trunc_f32_s" (f32.const 9223372036854775808.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_s" (f32.const -9223373136366403584.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_s" (f32.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_s" (f32.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i64.trunc_f32_u" (f32.const 18446744073709551616.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_u" (f32.const -1.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_u" (f32.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f32_u" (f32.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i64.trunc_f64_s" (f64.const 9223372036854775808.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_s" (f64.const -9223372036854777856.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_s" (f64.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_s" (f64.const -inf)) "error_code: 90002\nerror_msg: integer overflow")

(assert_trap (invoke "i64.trunc_f64_u" (f64.const 18446744073709551616.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_u" (f64.const -1.0)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_u" (f64.const inf)) "error_code: 90002\nerror_msg: integer overflow")
(assert_trap (invoke "i64.trunc_f64_u" (f64.const -inf)) "error_code: 90002\nerror_msg: integer overflow")
