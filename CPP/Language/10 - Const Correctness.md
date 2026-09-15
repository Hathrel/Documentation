# Const Correctness

`const` expresses and enforces non-mutation through a name or interface.

`const` qualifies a type and restricts operations through that access path. It does not necessarily mean the underlying bytes can never change: another non-const alias may modify a non-const object, and a const class may contain mutable synchronization or cache state.

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

## Top-level and low-level const

Top-level const qualifies the object itself; low-level const appears within a compound type:

```cpp
const int value = 1;  // top-level const on value
const int* pointer;   // low-level const on pointed-to int
int* const fixed = p; // top-level const on pointer
```

By-value parameter types ignore top-level const when forming a function signature, because callers cannot observe whether the callee mutates its private copy. Low-level const remains significant:

```cpp
void read(int);           // same signature as void read(const int)
void inspect(int*);
void inspect(const int*); // distinct overload
```

Template deduction and `auto` commonly discard top-level const on copied values but preserve const reached through references or pointers.

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

The unsafe-looking conversion from `T**` to `const T**` is not allowed. If it were, code could store a pointer to a truly const object into a `T*` and later modify that object. Const qualification across multiple pointer levels follows stricter similarity rules than the single-level conversion.

## Const member functions

Inside a const member function, `this` points to const, so ordinary data members cannot be modified and only const-qualified member functions can be called on them. Const and non-const overloads can expose matching access:

```cpp
class Buffer {
public:
    char& at(std::size_t i) { return data_.at(i); }
    const char& at(std::size_t i) const { return data_.at(i); }
private:
    std::vector<char> data_;
};
```

Returning a mutable pointer or reference from a const member usually breaks logical constness, even if the type system permits it through an indirection.

## `mutable` and logical constness

`mutable` exempts a data member from the const restriction on the containing object. Appropriate uses include a mutex, memoized cache, or diagnostic counter that does not change the object's externally meaningful value. Synchronize caches correctly; `const` alone says nothing about thread safety.

A lambda's call operator is const by default. A lambda marked `mutable` may modify its by-value captures; this does not modify the original captured objects.

## Related but different qualifiers

- `constexpr` means a value or function can participate in constant evaluation under its rules; a constexpr object is also const.
- `consteval` requires calls to be evaluated at compile time.
- `constinit` requires static initialization but does not make a variable const.
- `volatile` tells the implementation that accesses are observable in specific ways; it does not provide atomicity or inter-thread synchronization.

Use `const_cast` only when adapting an incorrectly const-qualified legacy interface or sharing an implementation between overloads with a proven contract. Modifying an object that was originally defined const is undefined behavior even after casting.
