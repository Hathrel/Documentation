# Lambdas and Callable Objects

A lambda creates an unnamed function object, useful for local behavior and algorithms.

```cpp
const int threshold{10};
const auto is_large = [threshold](int value) {
    return value > threshold;
};
```

## Capture

- `[]`: capture nothing.
- `[x]`: copy `x` into the closure.
- `[&x]`: hold a reference to `x`.
- `[=]` / `[&]`: implicitly capture used locals by copy/reference; explicit captures are clearer.
- `[value = expression]`: initialize a capture; can move ownership.

```cpp
auto task = [resource = std::move(resource)]() mutable {
    resource.use();
};
```

Reference captures must not outlive their referents, especially in callbacks, threads, and asynchronous work. Capturing `this` does not keep the object alive.

## Algorithm example

```cpp
std::ranges::sort(people, {}, &Person::last_name);

const auto count = std::ranges::count_if(people, [](const Person& p) {
    return p.active && p.age >= 18;
});
```

The optional parameter list can be omitted for no arguments: `[] { return 42; }`. `mutable` allows modification of by-value captures. A trailing return type handles cases where deduction is insufficient: `[](int x) -> double { ... }`.

## Callable types

Functions, function pointers, lambdas, and objects with `operator()` are callable. Templates usually accept callables with zero overhead. `std::function<Signature>` provides type erasure for a copyable stored callable but can allocate and adds indirection. Use it when runtime storage of heterogeneous callables is needed, not automatically for every callback.

Use `std::invoke` to uniformly call functions, member functions, and member pointers.

