# Error Handling

Errors need a policy. Separate programmer bugs (violated preconditions/invariants) from expected runtime failures (missing file, invalid input, network timeout).

## Main approaches

- **Exceptions:** failure is exceptional and cannot be handled locally; constructors can fail cleanly.
- **`std::optional<T>`:** absence is normal and needs no explanation.
- **`std::expected<T, E>` (C++23):** success or a typed, expected error.
- **Error code/result type:** useful at C boundaries, constrained systems, or exception-free codebases.
- **Assertions:** detect programmer mistakes; not input validation.

## Exceptions

Throwing creates an exception object and transfers control to the nearest matching handler after stack unwinding. Matching is primarily type-based; derived exceptions are caught by compatible base references. Catch by const reference to avoid slicing and copying.

```cpp
double divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        throw std::invalid_argument{"denominator must not be zero"};
    }
    return numerator / denominator;
}

try {
    std::cout << divide(10, 0);
} catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << '\n';
}
```

Catch by `const&`, from specific to general. Do not catch only to ignore errors. Throw objects by value. Use RAII so cleanup is automatic. Destructors and ordinary swap/move operations should generally be `noexcept`.

A catch block can use `throw;` to rethrow the current exception without slicing it. Writing `throw error;` creates a new exception from the caught expression and can lose dynamic type information.

Exceptions should leave program invariants intact. Operations commonly promise the no-throw, strong, or basic exception guarantee; see [[Language/11 - Resource Management and RAII]]. Destructors, cleanup actions, and rollback operations are the foundation of those guarantees.

### `noexcept` and termination

If an exception escapes a `noexcept` function, the implementation calls `std::terminate`; callers cannot catch across that boundary. This is a contract and an optimization signal, not a request to ignore errors.

Use conditional `noexcept` in generic code when the operation depends on a type:

```cpp
template<class T>
void relocate(T& target, T& source)
    noexcept(std::is_nothrow_move_assignable_v<T>) {
    target = std::move(source);
}
```

Functions called during stack unwinding must not emit a second exception. If a destructor needs fallible finalization, expose an explicit operation that callers can check and keep the destructor's fallback non-throwing.

### Boundaries and diagnostics

Catch exceptions where code can add context, recover, translate to another error mechanism, or terminate a top-level operation. Do not catch and immediately rethrow without adding value. At thread, callback, C ABI, and process entry boundaries, prevent exceptions from escaping into an environment that does not support them.

Nested exceptions or source-location data can preserve causal context. Error messages should identify the failed operation and relevant safe inputs; the exception type should describe the category callers can act on.

## Value-based errors

`std::optional<T>` represents presence or absence only. Use a result/expected type when the reason for failure matters. `std::expected<T, E>` (C++23) stores either a value or a typed error and makes failure visible in the return type.

Choose an error type with stable programmatic meaning rather than forcing callers to parse prose. Error codes work well for domains with an established enumeration and for boundaries where exceptions cannot cross. Never ignore a returned error merely because the language permits it; apply `[[nodiscard]]` to important result types.

## Optional

```cpp
std::optional<int> parse_small_int(std::string_view text);

if (const auto value = parse_small_int(input)) {
    use(*value);
} else {
    report_invalid();
}
```

Do not use magic sentinel values (`-1`) when every underlying value might be valid.

## Assertions

```cpp
#include <cassert>
assert(index < values.size());
```

`assert` may disappear when `NDEBUG` is defined. Never put required side effects inside it, and never use it as the only check on untrusted input.

Document whether APIs throw, return failures, terminate, or require preconditions. Keep one coherent policy across a component.

## Preconditions are not runtime failures

A precondition states what callers must provide for the operation to be valid. Violating one is a programmer error, not an alternate result. Assertions and contract-checking facilities can detect violations during development, but unchecked standard-library preconditions may lead directly to undefined behavior.

Validate untrusted data because invalid external input is expected at runtime. Do not rely on `assert` for validation, security, resource limits, or cleanup. Prefer an ordinary branch returning or throwing a documented error.
