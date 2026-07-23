# Templates and Concepts

Templates describe families of functions or types. The compiler instantiates concrete code for the types used.

## Function templates

```cpp
template<class T>
T max_value(const T& a, const T& b) {
    return a < b ? b : a;
}

auto larger = max_value(3, 7); // T is int
```

Both values must deduce the same `T`. Templates use duck typing unless constrained: errors emerge when required operations are missing.

## Concepts (C++20)

Concepts name requirements and improve overload selection and diagnostics.

```cpp
#include <concepts>

template<std::integral T>
T gcd(T a, T b) { /* ... */ }

template<class T>
concept Printable = requires(std::ostream& out, const T& value) {
    { out << value } -> std::same_as<std::ostream&>;
};

template<Printable T>
void print(const T& value) { std::cout << value; }
```

Constrain interfaces to the operations they truly require. Prefer standard concepts (`std::ranges::range`, `std::regular`, `std::invocable`) where they fit.

## Class templates

```cpp
template<class T>
class Box {
public:
    explicit Box(T value) : value_{std::move(value)} {}
    const T& get() const { return value_; }
private:
    T value_;
};

Box box{42}; // class template argument deduction
```

Template definitions normally live in headers because the compiler needs the complete definition at the point of instantiation.

## Specialization

Templates can be fully or partially specialized, but specialization rules are subtle. Prefer overloads, concepts, and composition until specialization gives a clear benefit. Never add specializations to `std` except where the standard explicitly permits it (for example, `std::hash` for a user-defined type under its requirements).

## Variadic templates

Parameter packs accept zero or more template arguments. Fold expressions combine them:

```cpp
template<class... Ts>
auto sum(Ts... values) { return (values + ...); }
```

Generic code multiplies both reuse and complexity. Keep its contract small and test it with varied types.

