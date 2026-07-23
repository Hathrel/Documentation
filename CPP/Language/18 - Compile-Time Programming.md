# Compile-Time Programming

Compile-time evaluation can enforce invariants, remove runtime work, and generate type-safe code. Use it where it clarifies intent; elaborate metaprogramming can make errors and builds worse.

## `constexpr` and `consteval`

```cpp
constexpr int square(int x) { return x * x; }
static_assert(square(5) == 25);

consteval unsigned long long operator"_KiB(unsigned long long n) {
    return n * 1024;
}
constexpr auto buffer_size = 4_KiB;
```

A `constexpr` function may run at compile time when inputs and context allow, or at runtime. A `consteval` immediate function must run at compile time. `constinit` ensures static/thread storage initialization is static but does not make the object const.

## Type traits

`<type_traits>` queries and transforms types:

```cpp
template<class T>
constexpr bool is_number_v = std::is_arithmetic_v<std::remove_cvref_t<T>>;
```

Prefer concepts for interface constraints; traits remain useful for implementation decisions.

## `if constexpr`

```cpp
template<class T>
void serialize(const T& value) {
    if constexpr (std::integral<T>) {
        write_integer(value);
    } else {
        value.serialize();
    }
}
```

The discarded branch is not instantiated for that specialization.

## Static assertions

Use `static_assert(condition, "message")` for assumptions that can be checked during compilation, such as type requirements and representation constraints.

Template instantiation can increase executable size and compile time. Measure rather than assuming compile-time work is automatically better.

