# Error Handling

Errors need a policy. Separate programmer bugs (violated preconditions/invariants) from expected runtime failures (missing file, invalid input, network timeout).

## Main approaches

- **Exceptions:** failure is exceptional and cannot be handled locally; constructors can fail cleanly.
- **`std::optional<T>`:** absence is normal and needs no explanation.
- **`std::expected<T, E>` (C++23):** success or a typed, expected error.
- **Error code/result type:** useful at C boundaries, constrained systems, or exception-free codebases.
- **Assertions:** detect programmer mistakes; not input validation.

## Exceptions

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

