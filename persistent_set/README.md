# Persistent set

- language: c++11
- tests: gtest

Implementation of fully persistent associative set with [path copying](https://en.wikipedia.org/wiki/Persistent_data_structure#Path_copying).
Persistency in that interface is expressed by O(1) complexity on copy. 
If you want to save your version of the set you must to manual copy that.

Under the hood, each instance of persistent set holds shared smart pointer on the root of BST. Internal implementation of modifying operation 
returns new root, after that new root replaces with old root. [
Smart pointers](https://github.com/GoPavel/cpp-course/tree/master/persistent_set/smart_pointers) 
between nodes and root provide memory safety and space leak safety.

Also repository contains two own implementation of the shared pointer: 
[shared_ptr](https://github.com/GoPavel/cpp-course/blob/master/persistent_set/smart_pointers/shared_ptr.h) that's similar std::shared_ptr
and
[linked_ptr](https://github.com/GoPavel/cpp-course/blob/master/persistent_set/smart_pointers/linked_ptr.h)

Сlass signature
```c++
template <typename T, template<typename> class Memory_manager_ptr = std::shared_ptr>
class persistent_set {
...
};
```

`T` - must be Destructible, LessThanComparable, CopyConstructible

`Memory_manager_ptr` - class of smart pointers which are used for referring on node of tree.


## Build

**requirements**: cmake

```bash
cmake .
make
```

## Tests

**requirements**: gtests

Contains 3 block of tests:
- [shared_ptr](https://github.com/GoPavel/cpp-course/blob/master/persistent_set/perset_testing.cpp#L17)
- [linked_ptr](https://github.com/GoPavel/cpp-course/blob/master/persistent_set/perset_testing.cpp#L79)
- [perset](https://github.com/GoPavel/cpp-course/blob/master/persistent_set/perset_testing.cpp#L162)
