# Const Correctness

`const` expresses and enforces non-mutation through a name or interface.

```cpp
void print(const Report& report);  // does not modify report

class Counter {
public:
    int value() const { return value_; } // callable on const objects
    void increment() { ++value_; }
private:
    int value_{};
};
```

Place `const` on variables by default when they do not need to change. It narrows the reader’s mental search space and prevents accidental writes.

## Pointers

```cpp
const int* p1;       // pointer to const int
int* const p2 = &x;  // const pointer to mutable int
const int* const p3 = &x;
```

Read declarations from the variable outward. Writing `int const*` is equivalent to `const int*`.

## Logical constness

A `const` member function promises not to change the object’s observable state. A `mutable` cache or mutex may change internally without changing logical value:

```cpp
mutable std::mutex mutex_;
```

Use `mutable` for genuinely non-observable implementation details, never to evade a poor interface.

## Const and return types

Returning `const T` by value is usually harmful because it can inhibit moves. Return `T`. Returning `const T&` is appropriate only when the referred object will outlive the caller’s use and exposing it does not break encapsulation.

`const` is not transitive through ordinary pointers: a const object that holds a pointer cannot change the pointer, but may still be able to change the pointed-to object. Choose pointer types that encode the intended access.

