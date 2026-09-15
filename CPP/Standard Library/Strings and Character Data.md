# Strings and Character Data

See [[Language/07 - Strings and Text]] for concepts and lifetime rules.

## Ownership, length, and termination

`std::string` owns a contiguous sequence; `std::string_view` borrows pointer plus length; a C string uses a null terminator. A view need not be terminated, and either sequence may contain embedded nulls.

Mutation can invalidate pointers, iterators, `c_str()` results, and views into a string. Small-string optimization is not a portable capacity or layout guarantee.

## `std::string`

`std::basic_string<CharT>` is an owning, contiguous, null-terminated character sequence; `std::string` uses `char`.

```cpp
std::string s{"alpha"};
s.append(" beta");
s.insert(0, "start: ");
s.replace(0, 5, "begin");
const auto position = s.find("beta");
if (position != std::string::npos) s.erase(position, 4);
```

`c_str()` and `data()` provide pointers for compatible APIs. Pointer/view/iterator validity can be lost when the string changes. Embedded null bytes are allowed in a string, but C APIs commonly stop at the first null.

## Views and spans

`std::string_view` represents read-only character data and may view literals, strings, or substrings without allocation. It is not necessarily null-terminated. `std::span<const char>` is more explicit when the input is arbitrary bytes/characters rather than semantic text.

Use a view parameter for synchronous read-only access when it will not be stored and no terminator is required. Return an owning string when producing text. Storing or returning a view requires an explicit source-lifetime contract.

## Character classification

Functions in `<cctype>` such as `std::isdigit` require either `EOF` or a value representable as `unsigned char`:

```cpp
const bool digit = std::isdigit(static_cast<unsigned char>(ch)) != 0;
```

They are locale-influenced and not general Unicode utilities.

## Conversion

- `std::to_chars` / `std::from_chars`: low-level, non-allocating number conversion with explicit error results.
- `std::to_string`: convenient number-to-string.
- `std::stoi`, `stod`, etc.: string-to-number, throwing and locale-influenced in specified ways.
- string streams: flexible but relatively heavy.

Prefer `from_chars` for robust parsers and check both the error code and whether all expected input was consumed.
