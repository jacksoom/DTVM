
# Mempool

The Mempool is responsible for allocating and deallocating memory, providing memory pool functionality. It can also create and destroy objects.


# Destroyer

The `Destroyer` is passed to `unique_ptr`. When the `unique_ptr` is destroyed, the `Destroyer` calls the Mempool to deallocate memory.


# MemPoolAllocater

The `MemPoolAllocator` provides an adapter layer for use in STL containers, enabling memory allocation and deallocation through the Mempool.


# Memory Management Practices

Taking `Module` as an example:

1. After a `Module` is created, it returns a `MemPoolUniquePtr`. The receiver does not need to manage the memory, as it will be automatically deallocated.
2. The `Module` internally creates additional memory, such as for `Function`, which is stored in a `vector`:
   1. The memory for the `vector` itself is managed using `MemPoolAllocator`.
   2. The memory for `Function` is managed within the `Module` using `MemPoolUniquePtr`. If smart pointers are not used, the memory must be explicitly released in the destructor.
3. For temporary memory that is not managed using `MemPoolUniquePtr`, you need to call `Mempool.Delete` or `Mempool.deallocate` to release it.
