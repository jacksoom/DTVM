;; the error case of invalid type-id when call_indirect

(module
  (memory 0)
  (type $sig (func))
  (func $f (result i32) (i32.const 123))
  (table funcref (elem $f))
  (func (export "test") (result i32) (call_indirect (type $sig) (i32.const 0)) (i32.const 1))
)

(assert_trap (invoke "test") "error_code: 90009\nerror_msg: TypeIdInvalid")
