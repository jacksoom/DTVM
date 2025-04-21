# WebAssembly to dMIR Mapping Guide

This document provides a comprehensive mapping between WebAssembly instructions and their corresponding dMIR representations. This mapping is essential for understanding how WebAssembly code is transformed into dMIR during compilation.

## Table of Contents

1. [Control Flow Instructions](#control-flow-instructions)
2. [Memory Instructions](#memory-instructions)
3. [Numeric Instructions](#numeric-instructions)
4. [Variable Instructions](#variable-instructions)
5. [Type Conversion Instructions](#type-conversion-instructions)
6. [Other Instructions](#other-instructions)

## Control Flow Instructions

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `unreachable` | `goto exception` | Unreachable code path |
| `nop` | - | No operation |
| `block` | Basic block | Structured control flow |
| `loop` | Basic block | Loop structure |
| `if` | `br_if` | Conditional branch |
| `else` | Basic block | Alternative path |
| `end` | Basic block end | Block termination |
| `br` | `br` | Unconditional branch |
| `br_if` | `br_if` | Conditional branch |
| `br_table` | `switch` | Table-based branch |
| `return` | `return` | Function return |

## Memory Instructions

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `i32.load` | `load` | Load 32-bit integer |
| `i64.load` | `load` | Load 64-bit integer |
| `f32.load` | `load` | Load 32-bit float |
| `f64.load` | `load` | Load 64-bit float |
| `i32.load8_s` | `load + sext` | Load 8-bit integer with sign extension |
| `i32.load8_u` | `load + uext` | Load 8-bit integer with zero extension |
| `i32.load16_s` | `load + sext` | Load 16-bit integer with sign extension |
| `i32.load16_u` | `load + uext` | Load 16-bit integer with zero extension |
| `i64.load8_s` | `load + sext` | Load 8-bit integer with sign extension |
| `i64.load8_u` | `load + uext` | Load 8-bit integer with zero extension |
| `i64.load16_s` | `load + sext` | Load 16-bit integer with sign extension |
| `i64.load16_u` | `load + uext` | Load 16-bit integer with zero extension |
| `i64.load32_s` | `load + sext` | Load 32-bit integer with sign extension |
| `i64.load32_u` | `load + uext` | Load 32-bit integer with zero extension |
| `i32.store` | `store` | Store 32-bit integer |
| `i64.store` | `store` | Store 64-bit integer |
| `f32.store` | `store` | Store 32-bit float |
| `f64.store` | `store` | Store 64-bit float |
| `i32.store8` | `trunc + store` | Store 8-bit integer |
| `i32.store16` | `trunc + store` | Store 16-bit integer |
| `i64.store8` | `trunc + store` | Store 8-bit integer |
| `i64.store16` | `trunc + store` | Store 16-bit integer |
| `i64.store32` | `trunc + store` | Store 32-bit integer |
| `memory.size` | `icall <memory size function>` | Get memory size |
| `memory.grow` | `icall <memory grow function>` | Grow memory |

## Numeric Instructions

### Integer Operations

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `i32.eqz` | `eqz` | Test if zero |
| `i32.eq` | `cmp` | Equal comparison |
| `i32.ne` | `cmp` | Not equal comparison |
| `i32.lt_s` | `cmp` | Signed less than |
| `i32.lt_u` | `cmp` | Unsigned less than |
| `i32.gt_s` | `cmp` | Signed greater than |
| `i32.gt_u` | `cmp` | Unsigned greater than |
| `i32.le_s` | `cmp` | Signed less than or equal |
| `i32.le_u` | `cmp` | Unsigned less than or equal |
| `i32.ge_s` | `cmp` | Signed greater than or equal |
| `i32.ge_u` | `cmp` | Unsigned greater than or equal |
| `i32.clz` | `clz` | Count leading zeros |
| `i32.ctz` | `ctz` | Count trailing zeros |
| `i32.popcnt` | `popcnt` | Population count |
| `i32.add` | `add` | Addition |
| `i32.sub` | `sub` | Subtraction |
| `i32.mul` | `mul` | Multiplication |
| `i32.div_s` | `sdiv` | Signed division |
| `i32.div_u` | `udiv` | Unsigned division |
| `i32.rem_s` | `srem` | Signed remainder |
| `i32.rem_u` | `urem` | Unsigned remainder |
| `i32.and` | `and` | Bitwise AND |
| `i32.or` | `or` | Bitwise OR |
| `i32.xor` | `xor` | Bitwise XOR |
| `i32.shl` | `shl` | Shift left |
| `i32.shr_s` | `sshr` | Signed shift right |
| `i32.shr_u` | `ushr` | Unsigned shift right |
| `i32.rotl` | `rotl` | Rotate left |
| `i32.rotr` | `rotr` | Rotate right |

### Floating-Point Operations

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `f32.abs` | `fpabs` | Absolute value |
| `f32.neg` | `fpneg` | Negation |
| `f32.ceil` | `fpround_ceil` | Ceiling |
| `f32.floor` | `fpround_floor` | Floor |
| `f32.trunc` | `fpround_trunc` | Truncate |
| `f32.nearest` | `fpround_nearest` | Round to nearest |
| `f32.sqrt` | `fpsqrt` | Square root |
| `f32.add` | `add` | Addition |
| `f32.sub` | `sub` | Subtraction |
| `f32.mul` | `mul` | Multiplication |
| `f32.div` | `fpdiv` | Division |
| `f32.min` | `fpmin` | Minimum |
| `f32.max` | `fpmax` | Maximum |
| `f32.copysign` | `fpcopysign` | Copy sign |

## Variable Instructions

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `local.get` | `dread <vreg>` | Get local variable |
| `local.set` | `<vreg> = ...` | Set local variable |
| `local.tee` | `<vreg> = ...` | Set and keep value |
| `global.get` | `load <global_addr>` | Get global variable |
| `global.set` | `store <global_addr>` | Set global variable |

## Type Conversion Instructions

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `i32.wrap_i64` | `trunc` | Wrap 64-bit to 32-bit |
| `i32.trunc_f32_s` | `wasm_fptosi` | Convert float to signed integer |
| `i32.trunc_f32_u` | `wasm_fptoui` | Convert float to unsigned integer |
| `i32.trunc_f64_s` | `wasm_fptosi` | Convert double to signed integer |
| `i32.trunc_f64_u` | `wasm_fptoui` | Convert double to unsigned integer |
| `i64.extend_i32_s` | `sext` | Sign extend 32-bit to 64-bit |
| `i64.extend_i32_u` | `uext` | Zero extend 32-bit to 64-bit |
| `f32.convert_i32_s` | `sitofp` | Convert signed integer to float |
| `f32.convert_i32_u` | `uitofp` | Convert unsigned integer to float |
| `f32.demote_f64` | `fptrunc` | Demote double to float |
| `f64.convert_i32_s` | `sitofp` | Convert signed integer to double |
| `f64.convert_i32_u` | `uitofp` | Convert unsigned integer to double |
| `f64.promote_f32` | `fpext` | Promote float to double |

## Other Instructions

| WASM Opcode | dMIR Opcode | Description |
|------------|------------|-------------|
| `call` | `call` | Direct function call |
| `call_indirect` | `icall` | Indirect function call |
| `select` | `select` | Conditional selection |
| `drop` | - | Drop value from stack |

## Notes

1. **Memory Access**
   - All memory access operations include bounds checking
   - Alignment requirements are enforced
   - Sign/zero extension is handled automatically for load operations

2. **Type Conversions**
   - Conversions between numeric types preserve value semantics
   - Floating-point conversions follow IEEE 754 rules
   - Integer conversions handle overflow/underflow appropriately

3. **Control Flow**
   - Basic blocks are created for structured control flow
   - Branch targets are resolved during lowering
   - Exception handling is supported for unreachable code

4. **Variables**
   - Local variables are assigned virtual registers
   - Global variables are accessed through memory operations
   - Variable lifetimes are tracked for optimization

## Future Extensions

The following WebAssembly features are planned for future support:
- SIMD instructions
- Threading operations
- Reference types
- Bulk memory operations
- Exception handling
