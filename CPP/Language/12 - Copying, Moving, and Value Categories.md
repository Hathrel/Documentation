# Copying, Moving, and Value Categories

Copying creates an independent value. Moving transfers resources from an object that will no longer be used for its old value.

```cpp
std::string a{"large payload"};
std::string b = a;            // copy; a unchanged
std::string c = std::move(a); // move; a remains valid but unspecified
```

`std::move` does not move anything itself; it marks an expression as eligible to bind to move operations. After moving from a standard-library object, you may destroy it, assign a new value, or perform operations without preconditions; do not assume its old contents.

## Value categories, practically

- **lvalue:** identifies a persistent object, usually has a name.
- **prvalue:** temporary/pure computed value, such as `Widget{}` or `a + b`.
- **xvalue:** expiring object, commonly produced by `std::move`.

Lvalue references `T&` bind to mutable lvalues. Rvalue references `T&&` bind to temporaries/xvalues. `const T&` can bind to both and may extend a directly bound temporary’s lifetime.

## Copy elision

Return local values normally:

```cpp
Widget make_widget() {
    Widget result{/*...*/};
    return result; // eligible for named return value optimization
}
```

Do not write `return std::move(result);`; it can prevent copy elision.

## Perfect forwarding

Generic wrapper code can preserve the caller’s value category:

```cpp
template<class T>
void relay(T&& value) {
    consume(std::forward<T>(value));
}
```

Here `T&&` is a forwarding reference because `T` is deduced. This is primarily library-author territory; ordinary code should favor clear value/reference interfaces.

Mark move operations `noexcept` when they truly cannot throw; containers can then move elements during reallocation.

