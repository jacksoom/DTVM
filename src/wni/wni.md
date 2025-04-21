WNI, ZetaEngine Wasm Native Interface
which provides vm related functions to native module, like address translation,
address validation, memory allocation/free, symbol allocation/free etc.

User could use helper headers providing by WNI to implement a native module quickly and easily.
and native module could exist in a library file(.so) or in a software module providing by
embedding host up to actual requirement. that means the native module is completely independent with vm.
VM will discover and load native modules based on WNI convention.
