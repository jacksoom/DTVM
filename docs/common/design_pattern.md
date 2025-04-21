# Design Patterns in ZetaEngine

This document describes the key design patterns used in the ZetaEngine project.

## PIMPL (Pointer to IMPLementation) Idiom

The PIMPL idiom is used to hide implementation details and reduce compilation dependencies.

### Key Benefits
- Reduces compilation dependencies
- Hides implementation details
- Improves binary compatibility
- Enables faster compilation

### Example Usage
See `lexer.h/cpp` and `parser.h/cpp` for implementation examples.

## Static Polymorphism

Static polymorphism is used for type-safe operations without runtime overhead.

### Key Features
- Compile-time type checking
- No virtual function overhead
- Type-safe operations

### Implementation Examples
See `type.h/cpp` and `function.h/cpp`:
- `Type` class hierarchy
- `FunctionType` implementation

### Polymorphism Methods
- `isa`: Type checking
- `cast`: Type casting
- `dynCast`: Dynamic type casting

## CRTP (Curiously Recurring Template Pattern)

CRTP is used for static polymorphism and compile-time interface implementation.

### Key Benefits
- Static polymorphism
- Compile-time interface implementation
- No virtual function overhead

### Implementation Examples
See the singlepass JIT implementation.

## Memory Management Patterns

### BumpPtrAllocator
- Fast allocation for temporary objects
- No deallocation overhead
- Used in Context and Type systems

### Memory Pool
- Efficient memory management
- Reduced fragmentation
- Used in IR construction

## Thread Safety Patterns

### Context-based Isolation
- Thread-local compilation state
- Parallel compilation support
- Memory management isolation

### Immutable Types
- Thread-safe type operations
- No synchronization needed
- Used in Type system

## Best Practices

1. **Type Safety**
   - Use static polymorphism where possible
   - Prefer compile-time checks
   - Avoid runtime type information

2. **Memory Management**
   - Use appropriate allocators
   - Follow RAII principles
   - Minimize dynamic allocation

3. **Thread Safety**
   - Use thread-local storage
   - Minimize shared state
   - Design for parallel execution
