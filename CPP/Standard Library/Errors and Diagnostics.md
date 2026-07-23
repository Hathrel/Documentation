# Errors and Diagnostics

The standard library offers several error representations; [[Language/13 - Error Handling]] explains policy choices.

## Exception hierarchy

`std::exception` is the common base for standard exceptions and exposes `what()`. Major families include:

- `std::logic_error`: `invalid_argument`, `domain_error`, `length_error`, `out_of_range`.
- `std::runtime_error`: `range_error`, `overflow_error`, `underflow_error`.
- other types such as `bad_alloc`, `bad_cast`, `bad_optional_access`, `filesystem_error`, and `system_error`.

The hierarchy labels are imperfect; choose a type callers can meaningfully distinguish, or define a focused project exception type.

## Error codes

`std::error_code` contains a numeric value plus an error category. It suits system and library operations where failures are values.

```cpp
std::error_code error;
const bool removed = std::filesystem::remove(path, error);
if (error) {
    std::cerr << error.message() << '\n';
}
```

Compare against typed conditions (`std::errc`) when possible rather than parsing messages. `std::system_error` carries an error code as an exception.

## Source and stack context

`std::source_location` (C++20) captures file, function, line, and column with low friction:

```cpp
void log(std::string_view message,
         std::source_location where = std::source_location::current());
```

`std::stacktrace` is standardized in C++23, but availability and symbol quality depend on the implementation, build symbols, and platform. Treat diagnostic text as operational data, not a stable machine-readable API.

## Termination

`std::terminate` runs when exception handling cannot continue, such as when an exception leaves a `noexcept` function. A terminate handler may log minimal context but cannot safely resume. `std::abort`, `std::exit`, `std::quick_exit`, and normal return have different cleanup behavior; prefer ordinary return and RAII, reserving abrupt termination for unrecoverable process states.

