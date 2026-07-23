# Documentation and API Design

An API is good when correct use is obvious and incorrect use is difficult.

## State the contract

Document:

- purpose and semantic meaning;
- preconditions and valid ranges;
- ownership and lifetime;
- units and encoding;
- mutation and thread-safety;
- error behavior and exception guarantees;
- complexity when relevant;
- iterator/reference invalidation;
- one realistic example.

```cpp
// Returns a view into `buffer`. The view is invalidated by any non-const
// operation on `buffer`. Throws std::out_of_range when offset > buffer.size().
std::string_view suffix(const std::string& buffer, std::size_t offset);
```

## Interface choices

- Prefer domain types to ambiguous booleans/integers.
- Prefer return values to output parameters.
- Accept the weakest sufficient abstraction (`span`, `string_view`, range, callable).
- Avoid exposing storage unless it is the abstraction.
- Preserve source compatibility deliberately; adding overloads or virtual functions can break consumers in subtle ways.
- Mark important results `[[nodiscard]]` and nonthrowing operations `noexcept` only when true.

## Header documentation

Keep public contracts near declarations and implementation rationale near definitions. Examples and tests should compile in CI so they do not decay. Tools such as Doxygen can generate sites from structured comments, but concise accurate contracts matter more than a tool-specific comment style.

