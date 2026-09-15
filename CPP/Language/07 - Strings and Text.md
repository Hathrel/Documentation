# Strings and Text

## Representation and terminology

C++ has no single universal “string” language type. A string is usually a sequence of code units stored by a library type or array:

- a string literal such as `"cat"` has type `const char[4]`; the fourth element is the null terminator;
- `std::string` owns a resizable sequence of `char` and also maintains a trailing null terminator for `c_str()`;
- `std::string_view` stores a pointer and length but owns no characters;
- a C string is a null-terminated character sequence, commonly accessed through `const char*`.

Length and null termination are different protocols. A view can contain embedded nulls and need not have a terminator at `data() + size()`. A string also may contain embedded nulls even though C APIs often stop at the first one.

## Choose a text type

- `std::string`: owns a mutable sequence of `char`.
- `std::string_view`: read-only non-owning view; cheap to copy, lifetime-sensitive.
- `const char*`: null-terminated C string; needed at C interfaces.
- `char`: one code unit, not necessarily one user-visible character.

```cpp
#include <string>
#include <string_view>

std::string greeting{"Hello"};
greeting += ", C++";

bool starts_with(std::string_view text, std::string_view prefix) {
    return text.starts_with(prefix); // C++20
}
```

String literals such as `"hello"` have static lifetime. `"hello"sv` requires `using namespace std::string_view_literals;` in a narrow scope.

Choose by ownership and mutation:

- return `std::string` when producing owned text;
- accept `std::string_view` for synchronous read-only text when no termination is required;
- accept `std::string&` when modifying the caller's string;
- use `const char*` when interoperating with an API whose contract is specifically a C string.

Do not store a view unless the class's contract makes the source lifetime clear. A view into a `std::string` can dangle after the string is destroyed, moved from, reassigned, or reallocated.

## Storage, capacity, and invalidation

`std::string` is a contiguous container. `size()` is the number of stored code units, `capacity()` is the amount of currently allocated room, `reserve()` can reduce reallocations, and `resize()` changes the number of elements. Implementations commonly use a small-string optimization, but its size and existence are not portable contracts.

Operations that reallocate invalidate pointers, references, iterators, and views into the old buffer. Even without reallocation, erasure and replacement can invalidate references to affected elements. Never cache `c_str()` across a mutation unless the operation's invalidation rules guarantee it.

## Common operations

```cpp
text.empty();
text.size();
text.find("needle");             // returns std::string::npos if absent
text.substr(pos, count);         // new owning string
text_view.substr(pos, count);    // new view
std::stoi("42");                // throws on invalid/range errors
std::to_string(42);
```

For robust numeric parsing without allocation, use `std::from_chars`; for formatting, use `std::format` where the implementation supports it. See [[Standard Library/Text, Formatting, and Regular Expressions]].

`find` returns an index, not an iterator. Compare with `std::string::npos`, whose unsigned value is the largest representable `size_type`. Check positions before arithmetic so wrapping cannot disguise “not found.”

Constructors taking a pointer often assume null termination, while pointer-plus-count or view overloads preserve embedded nulls. Know which overload is selected.

## Input

`operator>>` reads a whitespace-delimited token. `std::getline` reads a whole line.

```cpp
std::string line;
if (std::getline(std::cin, line)) {
    // use line
}
```

After formatted extraction, consume leading whitespace with `std::getline(std::cin >> std::ws, line)` when appropriate.

## C API boundaries

`text.c_str()` and `text.data()` provide a null-terminated buffer for a const string in modern C++. For writable C APIs, resize the string to adequate storage first, pass `data()`, then resize to the actual written length according to the API contract. Never let a C API write beyond `size()` merely because `capacity()` is larger.

A pointer returned by a C API may be borrowed, static, newly allocated, or valid only until the next call. Wrap the documented ownership convention immediately rather than guessing from the pointer type.

## Encodings

C++ strings store code units; they do not inherently understand Unicode grapheme clusters. UTF-8 in `std::string` is a common external representation, but indexing addresses bytes/code units, not necessarily characters. Use a dedicated Unicode library when you need normalization, case folding, grapheme iteration, or locale-sensitive rules. Avoid assuming `std::tolower(char)` is Unicode-aware; cast to `unsigned char` before the `<cctype>` functions to avoid undefined behavior for negative `char` values.

## Lifetime trap

```cpp
std::string_view bad() {
    return std::string{"temporary"}; // dangling immediately
}
```

Views do not keep their source alive.

Other common traps include returning a view to a local string, storing a view into a temporary produced by concatenation, and keeping a view while mutating its owner. String literals are safe view sources because they have static storage duration.
