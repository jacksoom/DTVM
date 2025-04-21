;; the error case of i32.load exceed memory addresses.
;; the test vm init memory is 1MB=1048576bytes, so use 1048576 + 1 to visit.

(module
  (memory 0)
  (func (export "test") (result i32) (i32.load (i32.const 1048577)))
)

(assert_trap (invoke "test") "error_code: 90007\nerror_msg: OutOfBoundsMemory")
