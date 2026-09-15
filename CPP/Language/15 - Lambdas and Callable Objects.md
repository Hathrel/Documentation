# Lambdas and Callable Objects

A lambda creates an unnamed function object, useful for local behavior and algorithms.

Each lambda expression creates a unique, unnamed **closure type**. Its captures become data members, and its function body becomes a call operator. Two textually identical lambda expressions still have different types.

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

`[this]` captures the pointer by value, not the object. `[*this]` copies the current object into the closure. A callback that may outlive the object should capture an owning smart pointer when shared lifetime is intended, or a weak pointer that it checks at invocation.

Default capture does not mean every visible variable is stored; only odr-used entities are captured. Prefer explicit captures in escaping lambdas because they expose lifetime and ownership at the definition site.

An init-capture creates a new closure member:

```cpp
auto task = [socket = std::move(socket), attempts = 0]() mutable {
    ++attempts;
    socket.send();
};
```

The original variable and the capture are distinct. A move-only capture makes the closure move-only.

## Parameters and return type

`auto` parameters make a generic lambda whose call operator is a template. Explicit template parameters are available since C++20:

```cpp
auto first = []<std::ranges::range R>(R&& range)
    -> decltype(auto) {
    return *std::ranges::begin(range);
};
```

That return can dangle when called with a temporary range; generic syntax does not remove lifetime obligations. A lambda is const-callable by default. `mutable` removes const from its call operator so by-value captures can change.

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

A captureless lambda converts to a compatible function pointer, which is useful for C callbacks that need no context. Capturing lambdas require storage for their state and cannot make that conversion.

`std::function` erases the concrete callable type and may allocate. Prefer a template parameter for immediate invocation and owning type erasure only when runtime storage of heterogeneous callables is required.

## Lifetime checklist

Before storing or returning a lambda, inspect every capture. Values have the closure's lifetime; references, pointers, `this`, spans, and views remain tied to external objects. Also inspect what the callback returns—a reference into a capture becomes invalid when the closure is destroyed.
