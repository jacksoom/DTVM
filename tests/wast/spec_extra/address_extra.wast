

(module
  (memory 1)
  (data (i32.const 0) "\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\f4\7f\01\00\00\00\00\00\fc\7f")
  ;; load
  (func (export "load_const_base-2_offset_1") (result i32)
     (i32.load offset=1 (i32.const -2))
    )
  (func (export "load_dynamic_base-2_offset_1") (param i32) (result i32)
       (i32.load offset=1 (local.get 0))
      )
  (func (export "load_const_base_very_large_offset_very_large") (result i32)
       ;; offset=INT32_MAX-1, addr=INT32_MAX+10
       (i32.load offset=2147483646 (i32.const 2147483657))
      )
   (func (export "load_dynamic_base_very_large_offset_very_large") (param i32) (result i32)
          ;; offset=INT32_MAX-1, addr=INT32_MAX+10
          (i32.load offset=2147483646 (local.get 0))
         )

   ;; store
    (func (export "store_const_base-2_offset_1")
        (i32.store offset=1 (i32.const -2) (i32.const 7))
       )
    (func (export "store_dynamic_base-2_offset_1") (param i32)
          (i32.store offset=1 (local.get 0) (i32.const 7))
         )
   (func (export "store_const_base_very_large_offset_very_large")
      ;; offset=INT32_MAX-1, addr=INT32_MAX+10
      (i32.store offset=2147483646 (i32.const 2147483657) (i32.const 7))
     )
   (func (export "store_dynamic_base_very_large_offset_very_large") (param i32)
         ;; offset=INT32_MAX-1, addr=INT32_MAX+10
         (i32.store offset=2147483646 (local.get 0) (i32.const 7))
        )
  )
(assert_trap (invoke "load_const_base-2_offset_1") "out of bounds memory access")
(assert_trap (invoke "load_dynamic_base-2_offset_1" (i32.const -2)) "out of bounds memory access")
(assert_trap (invoke "load_const_base_very_large_offset_very_large") "out of bounds memory access")
(assert_trap (invoke "load_dynamic_base_very_large_offset_very_large" (i32.const 2147483657)) "out of bounds memory access")

(assert_trap (invoke "store_const_base-2_offset_1") "out of bounds memory access")
(assert_trap (invoke "store_dynamic_base-2_offset_1" (i32.const -2)) "out of bounds memory access")
(assert_trap (invoke "store_const_base_very_large_offset_very_large") "out of bounds memory access")
(assert_trap (invoke "store_dynamic_base_very_large_offset_very_large" (i32.const 2147483657)) "out of bounds memory access")
