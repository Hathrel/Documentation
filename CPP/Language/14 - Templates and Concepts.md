# Templates and Concepts

Templates describe families of functions or types. The compiler instantiates concrete code for the types used.

A template is not itself a function or class. It is a pattern plus rules for producing specializations. Parsing, name lookup, deduction, constraint checking, overload resolution, and instantiation happen at different points; many “template errors” make sense only when those stages are separated.

## Function templates

```cpp
template<class T>
T max_value(const T& a, const T& b) {
    return a < b ? b : a;
}

auto larger = max_value(3, 7); // T is int
```

Both values must deduce the same `T`. Templates use duck typing unless constrained: errors emerge when required operations are missing.

Template argument deduction matches argument types against parameter patterns. It does not generally perform conversions while deducing:

```cpp
template<class T>
void same(T, T);

same(1, 2);   // T is int
// same(1, 2.0); // conflict: int versus double
same<double>(1, 2.0); // explicit T permits conversion of 1
```

For by-value parameters, top-level const and references are discarded. Reference parameters preserve more cv and value-category information. Array and function decay depends on whether the parameter is by value or reference.

## Instantiation and name lookup

Template definitions normally live in headers because every translation unit that instantiates a specialization must see the definition. Explicit instantiation can centralize selected specializations in a source file, but then the supported type set is deliberately limited.

Names that do not depend on template parameters are looked up when the template is defined. Dependent names are resolved when instantiated. The parser sometimes needs help:

```cpp
template<class T>
void use(T& object) {
    typename T::value_type value{}; // dependent type
    object.template convert<int>(); // dependent template
}
```

This **two-phase lookup** can reveal portability bugs when one compiler delays checks more than another. Do not depend on declarations that are invisible at the template definition point unless argument-dependent lookup is intentionally involved.

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

A requires-expression checks whether expressions are valid; it does not normally execute them:

```cpp
template<class T>
concept Reservable = requires(T value, std::size_t n) {
    value.reserve(n);
    { value.size() } -> std::convertible_to<std::size_t>;
    requires std::movable<T>;
};
```

Constraints participate in overload selection and can order otherwise similar templates. Define concepts in semantic terms: syntactic validity alone cannot prove laws such as equality being transitive or copying producing an independent equivalent value.

Before concepts, substitution failure in an immediate context could remove an invalid candidate—SFINAE. You will encounter `std::enable_if` and detection idioms in older code, but concepts usually express the same intent more clearly.

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

Class-template argument deduction uses constructors and deduction guides to infer template arguments. It is not always the same as function-template deduction, and braces may select an initializer-list constructor. Spell arguments explicitly when deduction obscures ownership or representation.

## Specialization

Templates can be fully or partially specialized, but specialization rules are subtle. Prefer overloads, concepts, and composition until specialization gives a clear benefit. Never add specializations to `std` except where the standard explicitly permits it (for example, `std::hash` for a user-defined type under its requirements).

Function templates cannot be partially specialized; overload them instead. Explicit specializations must obey placement and visibility rules, and all translation units must agree about which specialization applies. A customization point or constrained overload is often safer.

## Variadic templates

Parameter packs accept zero or more template arguments. Fold expressions combine them:

```cpp
template<class... Ts>
auto sum(Ts... values) { return (values + ...); }
```

Generic code multiplies both reuse and complexity. Keep its contract small and test it with varied types.

Pack expansion applies a pattern to every element of a parameter pack. Fold expressions specify association and an optional identity; empty packs are valid only for operators/forms with defined identities or a supplied initial value.

## Generic-code design

Use templates when behavior is genuinely uniform across a family of types. Every operation in the implementation becomes an implicit or explicit requirement. Minimize those requirements, constrain the public boundary, and keep diagnostics close to the call.

Templates can increase compile time, binary size, and coupling because implementation is exposed. They also enable inlining and static polymorphism. Choose them for type-generic semantics, not automatically for every reusable function.
