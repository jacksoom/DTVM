;; the error case of i32.store exceed memory addresses.
;; the test vm init memory is 1MB=1048576bytes, so use 1048576 + 1 to visit.

(module
  (memory 0)
  (func (export "test") (result i32) (i32.store (i32.const 1048577) (i32.const 1)) (i32.const 1))
)

(assert_trap (invoke "test") "error_code: 90007\nerror_msg: OutOfBoundsMemory")
