# Strings and Text

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

## Input

`operator>>` reads a whitespace-delimited token. `std::getline` reads a whole line.

```cpp
std::string line;
if (std::getline(std::cin, line)) {
    // use line
}
```

After formatted extraction, consume leading whitespace with `std::getline(std::cin >> std::ws, line)` when appropriate.

## Encodings

C++ strings store code units; they do not inherently understand Unicode grapheme clusters. UTF-8 in `std::string` is a common external representation, but indexing addresses bytes/code units, not necessarily characters. Use a dedicated Unicode library when you need normalization, case folding, grapheme iteration, or locale-sensitive rules. Avoid assuming `std::tolower(char)` is Unicode-aware; cast to `unsigned char` before the `<cctype>` functions to avoid undefined behavior for negative `char` values.

## Lifetime trap

```cpp
std::string_view bad() {
    return std::string{"temporary"}; // dangling immediately
}
```

Views do not keep their source alive.

