# Coding Guidelines

These defaults make ownership, lifetime, and intent visible. A team’s documented conventions win when consistent.

## Design

- Keep functions short enough to explain with one sentence; give them one level of abstraction.
- Make invalid states hard to represent with types, constructors, and scoped enums.
- Prefer value semantics and composition. Use inheritance for substitutable runtime interfaces.
- Keep interfaces small; hide implementation details.
- Represent ownership explicitly. Raw pointers/references normally observe, not own.
- Use RAII for every resource.
- Prefer the rule of zero.
- Report errors consistently; do not silently continue with corrupted state.

## Code

- Initialize every object; prefer braces when they prevent narrowing.
- Use `const` by default and the narrowest scope possible.
- Use `auto` when the type is evident or needlessly verbose; spell out types that communicate units, ownership, or conversions.
- Prefer `enum class`, `nullptr`, range-based loops, standard algorithms, and standard containers.
- Use `std::vector` as the default sequence.
- Avoid macros except include guards and unavoidable platform/configuration integration. Use `constexpr`, inline functions, templates, and enums.
- Avoid `using namespace` in headers.
- Treat warnings as errors in CI once the baseline is clean.

## Naming example

```cpp
class HttpClient {
public:
    Response send(const Request& request);
private:
    std::chrono::milliseconds timeout_{5'000};
};
```

Choose a convention (`snake_case`, `camelCase`, etc.) and apply it consistently. Name variables for their domain role and include units where the type does not: `timeout_ms` is better than `t`.

## Comments

Explain *why*, constraints, ownership, units, and non-obvious tradeoffs. Do not narrate obvious syntax. Delete stale comments. Record preconditions and lifetime requirements near interfaces.

## Resist premature cleverness

Start with straightforward correct code. Abstract after duplication reveals a stable concept. Measure before optimizing. “Fewer lines” is not the same as “simpler.”

