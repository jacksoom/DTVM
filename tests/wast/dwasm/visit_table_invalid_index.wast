;; the error case of visit invalid table index

(module
  (memory 0)
  (type $sig (func))
  (func $f (result i32) (i32.const 123))
  (table funcref (elem $f))
  (func (export "test") (result i32) (call_indirect (type $sig) (i32.const 1)) (i32.const 1))
)

(assert_trap (invoke "test") "error_code: 90010\nerror_msg: TableElementIndexInvalid")
