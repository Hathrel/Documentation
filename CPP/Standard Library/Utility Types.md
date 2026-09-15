# Utility Types

These types model common shapes of data and make states explicit.

## Vocabulary types encode states

These types make alternatives visible but do not choose an ownership policy for referenced data. An optional view can still dangle.

## Optional, variant, any

```cpp
std::optional<User> find_user(Id id);           // zero or one User
std::variant<Text, Image, Video> content;        // exactly one listed type
std::any plugin_value;                           // any copyable type, runtime checked
```

Use `std::visit` for variants:

```cpp
std::visit([](const auto& value) { render(value); }, content);
```

Prefer `variant` over type-unsafe unions and usually over `any` when alternatives are known. `std::monostate` can represent an empty/default alternative.

`optional::value()` throws when disengaged while `operator*` has a precondition. `variant` holds one active alternative and can rarely become `valueless_by_exception`. `get` throws for the wrong alternative; `get_if` returns a pointer. Prefer variant over `any` for a closed set because visitation can be checked exhaustively.

## Pair and tuple

```cpp
std::pair<std::string, int> entry{"apples", 3};
auto [name, count] = entry; // structured binding
```

`tuple` holds heterogeneous values. Prefer a named struct when fields have domain meaning; `result.score` is clearer than `std::get<2>(result)`.

## Expected (C++23)

`std::expected<T, E>` holds either a result or an error. It is useful when failure is expected and callers must handle it explicitly. `std::unexpected(error)` creates the error alternative.

## Other utilities

- `std::move`, `forward`, `swap`, `exchange`: value/ownership plumbing.
- `std::reference_wrapper<T>` / `std::ref`: storable, copyable reference-like wrapper.
- `std::function`: type-erased copyable callable.
- `std::bind_front` (C++20): bind leading function arguments; a lambda is often clearer.
- `std::source_location` (C++20): caller file, line, column, function for logging/errors.
- `std::span`: non-owning contiguous range.
- `std::mdspan` (C++23): multidimensional view over data.

Each wrapper has lifetime and cost implications; choose it to express a real state, not to avoid designing a type.
