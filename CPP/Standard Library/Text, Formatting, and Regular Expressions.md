# Text, Formatting, and Regular Expressions

## Formatting

`std::format` (C++20) provides type-safe formatting where implemented:

```cpp
#include <format>
auto message = std::format("{:<12} {:08x} {:.2f}", name, id, price);
```

Format specifications control alignment, width, fill, sign, precision, and presentation. `std::format_to` writes through an output iterator. `std::print`/`println` are C++23 conveniences.

Never treat untrusted text as a compile-time format string. Use the runtime-format mechanism available in your standard/library version, or supply it as a value argument.

## Regular expressions

`<regex>` supports ECMAScript syntax by default and operations for match, search, replace, and iteration.

```cpp
const std::regex pattern{R"(^[A-Za-z_][A-Za-z0-9_]*$)"};
const bool valid = std::regex_match(identifier, pattern);
```

- `regex_match`: entire input matches.
- `regex_search`: a matching substring exists.
- `regex_replace`: creates replaced text.
- `std::smatch`: captures when input is `std::string`.

Compile reusable regex objects once. Catch `std::regex_error` when patterns are dynamic. Standard regex performance and feature behavior varies; use a dedicated library for high-throughput, adversarial, or advanced regex requirements.

## When not to use regex

Simple prefix/suffix/delimiter work is clearer with `string_view` operations. Structured languages generally need a parser, not one large regular expression.

Locale facilities (`<locale>`) support facets and locale-aware operations but are complex and not a complete modern Unicode solution.

