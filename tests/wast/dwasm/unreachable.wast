;; the error case of unreachable

(module
  (memory 0)
  (func (export "test") (unreachable))
)

(assert_trap (invoke "test") "error_code: 90012\nerror_msg: Unreachable")
