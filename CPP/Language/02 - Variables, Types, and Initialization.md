# Variables, Types, and Initialization

A variable is a named object with a type, value, storage location, scope, and lifetime.

## Fundamental types

```cpp
bool enabled{true};
char grade{'A'};
int count{42};
long long large{9'000'000'000LL};
float ratio{0.5F};
double precise{3.141592653589793};
```

Integer widths vary by platform. Use `std::int32_t`, `std::uint64_t`, and related types from `<cstdint>` only when exact widths matter (file formats, protocols). `std::size_t` from `<cstddef>` is the unsigned type used for sizes and indexes.

Floating-point values approximate real numbers. Do not test results for exact equality when rounding may occur; compare within a problem-appropriate tolerance.

## Initialize every object

Prefer brace initialization because it rejects many narrowing conversions:

```cpp
int uninitialized;       // indeterminate local value: dangerous
int zero{};              // 0
int answer{42};
double widened{answer};
// int narrowed{3.14};    // compile error
```

Use `const` when a value should not change and `constexpr` when it can be computed at compile time:

```cpp
const int input = read_value();
constexpr double pi{3.141592653589793};
```

## Type inference

`auto` asks the compiler to infer a type. Use it when the type is obvious from the initializer or verbose—not to hide important semantics.

```cpp
auto retries = 3;                    // int
auto name = std::string{"Ada"};     // std::string
const auto size = name.size();       // std::size_t
```

Plain `auto` drops top-level `const` and references. Use `const auto&` to observe without copying and `auto&` to mutate an existing object.

## Scope and lifetime

A name declared inside `{}` is visible from its declaration to the block’s end. Its automatic object is destroyed at block exit. Prefer the smallest useful scope.

Type conversions and casts are covered in [[Language/03 - Expressions and Operators]]. Ownership and lifetime are covered in [[Language/06 - References, Pointers, and Lifetimes]].

