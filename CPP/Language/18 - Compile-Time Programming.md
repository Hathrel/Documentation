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

## Constant-expression contexts

`constexpr` does not guarantee compile-time execution by itself. Constant evaluation is required in contexts such as `static_assert` conditions, non-type template arguments, array bounds, and initialization of a constexpr object. A constexpr function can also run normally at runtime.

During constant evaluation, operations that would have undefined behavior, forbidden runtime dependencies, or disallowed side effects make the expression non-constant. Modern C++ has progressively allowed more language features, but allocation and mutation used during evaluation must still satisfy constant-evaluation lifetime rules.

`consteval` declares an immediate function whose potentially evaluated calls must produce constant expressions. `constinit` applies to static or thread storage and prevents dynamic initialization; it does not imply constness.

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

Traits expose a compile-time value or type. Transformation traits often require `typename` for their nested `type`; aliases such as `std::remove_cvref_t<T>` avoid that ceremony. Prefer variable templates ending in `_v` for boolean values.

Do not infer semantic guarantees from weak structural traits. Trivially copyable does not mean safely serializable, and constructible does not mean construction preserves the domain invariant.

## Static assertions

Use `static_assert(condition, "message")` for assumptions that can be checked during compilation, such as type requirements and representation constraints.

Template instantiation can increase executable size and compile time. Measure rather than assuming compile-time work is automatically better.

## Compile-time dispatch versus generation

`if constexpr` selects among implementations inside one specialization. Constraints select among overloads before instantiation. Specialization replaces a template for particular arguments. Prefer the mechanism that most directly expresses the design.

Compile-time computation shifts work into builds and can multiply diagnostics and generated code. Use it to enforce types, configure fixed structures, and precompute genuinely constant data—not merely to avoid a cheap runtime branch.
