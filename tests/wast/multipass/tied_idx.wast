(module
  (type (;0;) (func (param i32 i32 i32) (result i32)))
  (type (;1;) (func (param i32)))
  (type (;2;) (func (param i32 i32 i32 i32) (result i32)))
  (type (;3;) (func (result i32)))
  (type (;4;) (func (param i32 i32 i32 i32 i32 i32 i32) (result i32)))
  (type (;5;) (func (param i32) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func (;0;) (type 1)))
  (import "wasi_snapshot_preview1" "fd_write" (func (;1;) (type 2)))
  (func (;2;) (type 3) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32)
    i32.const 1
    local.set 0
    i32.const 2
    local.set 1
    i32.const 3
    local.set 2
    i32.const 4
    local.set 3
    i32.const 5
    local.set 4
    global.get 0
    i32.const 208
    i32.sub
    local.tee 5
    global.set 0
    local.get 5
    local.get 2
    i32.store offset=204
    i32.const 0
    local.set 2
    local.get 5
    i32.const 160
    i32.add
    i32.const 0
    i32.const 40
    call 4
    drop
    local.get 5
    local.get 5
    i32.load offset=204
    i32.store offset=200
    block  ;; label = @1
      block  ;; label = @2
        i32.const 0
        local.get 1
        local.get 5
        i32.const 200
        i32.add
        local.get 5
        i32.const 80
        i32.add
        local.get 5
        i32.const 160
        i32.add
        local.get 3
        local.get 4
        call 5
        i32.const 0
        i32.ge_s
        br_if 0 (;@2;)
        i32.const -1
        local.set 1
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 0
        i32.load offset=76
        i32.const 0
        i32.lt_s
        br_if 0 (;@2;)
        local.get 0
        call 6
        local.set 2
      end
      local.get 0
      i32.load
      local.set 6
      block  ;; label = @2
        local.get 0
        i32.load8_s offset=74
        i32.const 0
        i32.gt_s
        br_if 0 (;@2;)
        local.get 0
        local.get 6
        i32.const -33
        i32.and
        i32.store
      end
      local.get 0
      i32.const 1
      i32.const 1
      call 0
      local.get 6
      i32.const 32
      i32.and
      local.set 6
      block  ;; label = @2
        block  ;; label = @3
          local.get 6
          local.get 1
          local.get 3
          local.get 4
          call 1
          i32.load offset=48
          i32.eqz
          br_if 0 (;@3;)
          local.get 0
          local.get 1
          local.get 5
          i32.const 200
          i32.add
          local.get 5
          i32.const 80
          i32.add
          local.get 5
          i32.const 160
          i32.add
          local.get 3
          local.get 4
          call 5
          local.set 1
          br 1 (;@2;)
        end
        local.get 0
        i32.const 80
        i32.store offset=48
        local.get 0
        local.get 5
        i32.const 80
        i32.add
        i32.store offset=16
        local.get 0
        local.get 5
        i32.store offset=28
        local.get 0
        local.get 5
        i32.store offset=20
        local.get 0
        i32.load offset=44
        local.set 7
        local.get 0
        local.get 5
        i32.store offset=44
        local.get 0
        local.get 1
        local.get 5
        i32.const 200
        i32.add
        local.get 5
        i32.const 80
        i32.add
        local.get 5
        i32.const 160
        i32.add
        local.get 3
        local.get 4
        call 5
        local.set 1
        local.get 4
        local.get 7
        local.get 4
        local.get 1
        call 1
        i32.eqz
        br_if 0 (;@2;)
        local.get 0
        i32.const 0
        i32.const 0
        local.get 0
        i32.load offset=36
        call_indirect (type 0)
        drop
        local.get 0
        i32.const 0
        i32.store offset=48
        local.get 0
        local.get 7
        i32.store offset=44
        local.get 4
        i32.const 0
        i32.store offset=28
        local.get 0
        i32.const 0
        i32.store offset=16
        local.get 0
        i32.load offset=20
        local.set 3
        local.get 0
        i32.const 0
        i32.store offset=20
        local.get 1
        i32.const -1
        local.get 3
        select
        local.set 1
      end
      local.get 0
      local.get 0
      i32.load
      local.tee 3
      local.get 6
      i32.or
      drop
      drop
      i32.const 794352029
      i32.const -1151769559
      i32.const 1475171889
      i32.const 1967522241
      i64.const 196398883
      i64.store offset=21 align=1
      i32.const 1879622632
      i32.const 786349434
      i64.const 2078005765
      i64.store offset=121 align=1
      i32.and
      select
      local.set 1
      local.get 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      call 7
      drop
      drop
    end
    local.get 5
    i32.const 208
    i32.add
    global.set 0
    local.get 1)
  (func (;3;) (type 0) (param i32 i32 i32) (result i32)
    i32.const 1)
  (func (;4;) (type 0) (param i32 i32 i32) (result i32)
    i32.const 1)
  (func (;5;) (type 4) (param i32 i32 i32 i32 i32 i32 i32) (result i32)
    i32.const 1)
  (func (;6;) (type 5) (param i32) (result i32)
    i32.const 1)
  (func (;7;) (type 1) (param i32))
  (table (;0;) 7 7 funcref)
  (memory (;0;) 256 256)
  (global (;0;) (mut i32) (i32.const 5245968))
  (global (;1;) (mut i32) (i32.const 0))
  (global (;2;) (mut i32) (i32.const 0))
  (export "body" (func 2)))

  (assert_return (invoke "body") (i32.const 0))
