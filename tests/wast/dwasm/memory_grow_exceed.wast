;; the edge error case of memory_grow testcase. 1page is 64 * 1024 bytes. the test max-memory = 16MB=256 pages.
;; init memory is 1MB=16 pages

(module
  (memory 0)
  (func (export "test") (result i32) (memory.grow (i32.const 257)))
)

(assert_return (invoke "test") (i32.const -1))