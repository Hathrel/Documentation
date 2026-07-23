# References, Pointers, and Lifetimes

Most serious C++ bugs are lifetime or ownership bugs. Ask of every pointer-like value: **what object does it refer to, who owns that object, and can it outlive this use?**

## References

A reference is an alias for an existing object. It must be initialized and normally cannot be reseated.

```cpp
int value{10};
int& mutable_ref = value;
const int& view = value;
mutable_ref = 20;          // changes value
```

Use `T&` for a required, non-owning, mutable parameter and `const T&` for a required, non-owning, read-only parameter when copying would be undesirable.

## Pointers

A pointer stores an address and may be null.

```cpp
int value{10};
int* ptr = &value;
if (ptr != nullptr) {
    *ptr = 20;             // dereference
}
```

Use a raw pointer primarily as an optional, non-owning observer. Express owning memory with values, containers, or smart pointers from [[Standard Library/Memory and Smart Pointers]]. Prefer `nullptr` to `0` or `NULL`.

## Lifetimes

```cpp
const std::string& bad() {
    std::string local{"gone soon"};
    return local; // dangling reference after return: undefined behavior
}
```

Common dangling cases:

- returning a pointer/reference/view to a local object;
- retaining a pointer after its owner is destroyed;
- keeping a `std::string_view` after the string dies or changes;
- using vector iterators/references after an operation that invalidates them;
- capturing a local by reference in a lambda that outlives the local.

## Arrays and pointer arithmetic

Built-in arrays often decay to pointers and lose size information. Prefer `std::array`, `std::vector`, or `std::span`. Pointer arithmetic is valid only within one array (plus its one-past-end position).

## Ownership vocabulary

- `T`: contained value; owns its resources.
- `T&`: required non-owning access.
- `T*`: optional non-owning access (by convention).
- `std::unique_ptr<T>`: exclusive ownership.
- `std::shared_ptr<T>`: shared lifetime; use sparingly.
- `std::weak_ptr<T>`: non-owning observation of shared ownership.

See [[Language/11 - Resource Management and RAII]] and [[Language/12 - Copying, Moving, and Value Categories]].

