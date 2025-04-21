# dMIR (Deterministic MIR) Intermediate Representation

## Overview

dMIR is a deterministic intermediate representation (IR) designed for the ZetaEngine WebAssembly runtime. It serves as a bridge between WebAssembly bytecode and machine code, providing a platform-independent representation that can be optimized and transformed before final code generation.

The design of dMIR is inspired by Maple IR, while its data structures are more influenced by LLVM. It is specifically designed to support deterministic execution, which is crucial for blockchain smart contract environments.

## Simple Examples

Here are some simple examples to help understand dMIR:

### Example 1: Simple Addition Function

```cpp
// WebAssembly function:
// (func (param i32) (param i32) (result i32)
//   (i32.add (local.get 0) (local.get 1)))

// dMIR representation:
func %add(i32, i32) -> i32 {
  %0 = dread i32 0  // Load first parameter
  %1 = dread i32 1  // Load second parameter
  %2 = add i32 %0, %1
  return i32 %2
}
```

### Example 2: If-Else Control Flow

```cpp
// WebAssembly function:
// (func (param i32) (result i32)
//   (if (result i32)
//     (i32.eqz (local.get 0))
//     (then (i32.const 1))
//     (else (i32.const 0))))

// dMIR representation:
func %is_zero(i32) -> i32 {
  %0 = dread i32 0
  %1 = eqz i32 %0
  br_if %1, %true, %false

%true:
  %2 = const i32 1
  return i32 %2

%false:
  %3 = const i32 0
  return i32 %3
}
```

### Example 3: Memory Access

```cpp
// WebAssembly function:
// (func (param i32) (result i32)
//   (i32.load (local.get 0)))

// dMIR representation:
func %load_memory(i32) -> i32 {
  %0 = dread i32 0
  %1 = load i32 %0
  return i32 %1
}
```

### Example 4: Function Call

```cpp
// WebAssembly function:
// (func (param i32) (param i32) (result i32)
//   (call $add (local.get 0) (local.get 1)))

// dMIR representation:
func %call_add(i32, i32) -> i32 {
  %0 = dread i32 0
  %1 = dread i32 1
  %2 = call i32 %add(%0, %1)
  return i32 %2
}
```

These examples demonstrate:
- Basic arithmetic operations
- Control flow with branches
- Memory access operations
- Function calls

The dMIR representation is designed to be both human-readable and machine-processable, making it easier to understand and manipulate the intermediate representation.

## Key Features

- **Deterministic Execution**: Ensures consistent behavior across different platforms
- **Type Safety**: Strong typing system with support for WebAssembly types
- **Optimization-Friendly**: Designed to support various optimization passes
- **Memory Efficient**: Uses memory pools for allocation and deallocation
- **Parallel Compilation**: Supports concurrent compilation through Context management

## IR Structure

### Module

The top-level container that holds all MIR functions in a compilation unit.

### Function

Represents a WebAssembly function with the following components:
- Function signature (parameters and return type)
- List of variables
- List of basic blocks
- Instructions

#### FunctionType
The function signature is represented by `FunctionType` with the following memory layout:
```
[FunctionType own members, return value Type*, parameter Type*...]
```

FunctionType instances are unique per signature and managed by the Context to avoid duplication.

### BasicBlock

A sequence of instructions with a single entry and exit point. Basic blocks are the fundamental units of control flow in dMIR.

### Variable

Variables in dMIR are not in SSA (Static Single Assignment) form by default, allowing multiple assignments. This design choice simplifies the initial IR construction while still supporting SSA transformation if needed.

### Instruction

Instructions are the basic operations in dMIR. They have the following characteristics:

- Memory layout: `[Operands Instruction*..., Instruction's own members]`
- Operands are encapsulated as Instructions (unlike LLVM's Value system)
- Support for static polymorphism through the `_kind` field
- Two main categories:
  - Statements: Can be linked in a BasicBlock
  - Expressions: Represent values and can be used as operands

#### Instruction Types

1. **Binary Instructions**
   - Arithmetic operations (add, sub, mul, etc.)
   - Bitwise operations (and, or, xor, etc.)
   - Comparison operations (cmp)

2. **Unary Instructions**
   - Type conversions
   - Arithmetic operations (neg, abs, etc.)
   - Control flow (br, br_if)

3. **Memory Instructions**
   - Load operations
   - Store operations
   - Memory access checks

4. **Control Flow Instructions**
   - Branch instructions (br, br_if)
   - Switch instructions
   - Return instructions

5. **Call Instructions**
   - Direct calls
   - Indirect calls
   - System calls

## Memory Management

dMIR uses a memory pool (`mempool`) for efficient allocation and deallocation of IR objects. When configured appropriately, this can eliminate the need for explicit cleanup of basic blocks, variables, and instructions.

## Context

The Context class manages thread-specific compilation state:
- Global types
- Function type deduplication
- Memory allocation

## Type System

dMIR supports the following types:
- Integer types (i32, i64)
- Floating-point types (f32, f64)
- Function types
- Pointer types

For detailed type information, see [type.md](./type.md).

## WebAssembly to dMIR Mapping

dMIR provides a direct mapping from WebAssembly instructions to dMIR operations. For the complete mapping, see [wasm_dmir.md](./wasm_dmir.md).

## Usage Example

```cpp
// Create a new function
MFunction *func = MFunction::create(...);

// Add a basic block
MBasicBlock *bb = MBasicBlock::create(...);
func->addBasicBlock(bb);

// Create instructions
MInstruction *add = BinaryInstruction::create(OP_add, type, lhs, rhs);
MInstruction *ret = ReturnInstruction::create(type, add);

// Add instructions to basic block
bb->addInstruction(add);
bb->addInstruction(ret);
```

## Best Practices

1. **Memory Management**
   - Use the provided memory pool for allocation
   - Avoid manual memory management when possible

2. **Type Safety**
   - Always specify correct types for instructions
   - Use type checking utilities when available

3. **Optimization**
   - Keep basic blocks small and focused
   - Minimize variable assignments
   - Use appropriate instruction types for operations

4. **Parallel Compilation**
   - Create separate Context instances for parallel compilation
   - Avoid sharing Context instances between threads

## Implementation Notes

- dMIR is designed to be both human-readable and machine-processable
- The IR supports various optimization passes
- Type information is preserved throughout the compilation process
- Memory layout is optimized for efficient access and manipulation

## Future Extensions

Planned enhancements for dMIR include:
- Support for more WebAssembly features
- Additional optimization passes
- Enhanced debugging capabilities
- Better integration with other compiler components
