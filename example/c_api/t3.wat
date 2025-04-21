(module
  (memory 1)
  (func $entry
    (call $call)
  )
  (func $call
    (call $call)
  )
  (export "entry" (func $entry))
)