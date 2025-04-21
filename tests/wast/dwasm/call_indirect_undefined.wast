;; the error case of found undefined in table when call_indirect

(module
  (memory 0)
  (type $sig (func))
  (func $f (result i32) (i32.const 123))
  (table 2 2 funcref)
  (elem (i32.const 0) $f)
  (func (export "test") (result i32) (call_indirect (type $sig) (i32.const 1)) (i32.const 1))
)

(assert_trap (invoke "test") "error_code: 90011\nerror_msg: CallIndirectTargetUndefined")
