dtvmcore_rust
===========

This is rust crate wrapper for DTVM-ZetaEngine. Users can use this crate to run WASM in Rust programs.

# System

* Linux: gcc 7+
* Mac: brew install gcc@11 and set gcc@11 as default gcc

# Usage

* step1: add dependency to your rust program's Cargo.toml

```
dtvmcore_rust = { path = "../DTVM/rust_crate" }
cty = "0.2.2"
libc = { version = "0.2.121" }
```

* step2: write wasm hostapis in Rust

```
use dtvmcore_rust::core::{
    host_module::*, instance::*, r#extern::*,
    types::*,
};

extern "C" fn get_host_number(wasm_inst: *mut ZenInstanceExtern, a: i32, b: i32) -> i32 {
    let inst = ZenInstance::from_raw_pointer(wasm_inst);
    assert!(inst.validate_wasm_addr(0, 1));
    let memory_addr: *const u8 = inst.get_host_memory(0);
    let memory_addr_value = unsafe { *memory_addr } as i32; // this memory data is asciiOf('a') = 97
    println!("memory_addr_value: {memory_addr_value}");
    println!("enter get_host_number, a={a}, b={b}");
    return 100000 + memory_addr_value + a + b;
}

```

* step3: run wasm by ZetaEngine APIs

```
let rt = ZenRuntime::new(None);
let rt_ref = rt.borrow();

// register hostapis
let host_func0 = ZenHostFuncDesc {
    name: "get_host_number".to_string(),
    arg_types: vec![ZenValueType::I32, ZenValueType::I32],
    ret_types: vec![ZenValueType::I32],
    ptr: get_host_number as *const cty::c_void,
};
let host_funcs = vec![host_func0];
let host_module = rt_ref.create_host_module("env", &host_funcs, true);
if let Err(err) = host_module {
    println!("host_module error: {err}");
    assert!(false);
    return;
}

let wasm_path = "./example/demo_hostapi.0.wasm";
let wasm_bytes = fs::read(wasm_path).unwrap();
println!("loading wasm module {wasm_path}");
let maybe_mod = rt_ref.load_module_from_bytes(wasm_path, &wasm_bytes);
if let Err(err) = maybe_mod {
    println!("load module error: {err}");
    assert!(false);
    return;
}
println!("load wasm module done");
let wasm_mod = maybe_mod.unwrap();
let isolation = rt_ref.new_isolation();
if let Err(err) = isolation {
    println!("create isolation error: {err}");
    assert!(false);
    return;
}
let mut isolation = isolation.unwrap();
let gas_limit: u64 = 100000000;
let maybe_inst = wasm_mod.new_instance(&mut isolation, gas_limit);
if let Err(err) = maybe_inst {
    println!("create wasm instance error: {err}");
    assert!(false);
    return;
}
println!("create wasm instance done");
let mut inst = maybe_inst.unwrap();
let args = vec![ZenValue::ZenI32Value(2), ZenValue::ZenI32Value(3)];
let results = inst.call_wasm_func("test", &args);
if let Err(err) = results.is_err() {
    println!("call wasm func error: {err}");
    assert!(false);
    return;
}
let result = &results.as_ref().unwrap()[0];
println!("wasm func test(2,3) result: {}", result.to_string());
println!(
    "wasm func results count: {}",
    &results.as_ref().unwrap().len()
);
assert_eq!("100102".to_string(), result.to_string());
```

# build

```
./build.sh
```

# development

* The cargo build will auto call bash build_cpp_lib.sh and copy_deps.sh in build.rs, so if you want the update the ZetaEngine directly, just call these scripts.
