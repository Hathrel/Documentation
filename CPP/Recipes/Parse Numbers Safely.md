# Parse Numbers Safely

`std::from_chars` avoids allocation, does not use locale, and reports errors explicitly.

```cpp
#include <charconv>
#include <optional>
#include <string_view>
#include <system_error>

std::optional<int> parse_int(std::string_view text) {
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size()) {
        return std::nullopt;
    }
    return value;
}
```

This rejects empty input, overflow, and trailing characters. It does not skip whitespace. Decide deliberately whether to trim, allow `+`, allow prefixes, or accept trailing units.

Range-check after parsing:

```cpp
auto port = parse_int(input);
if (!port || *port < 1 || *port > 65'535) {
    return error("port must be 1..65535");
}
```

Floating-point `from_chars` support depends on the standard library version. When unavailable, a carefully checked library parser or `std::strtod` wrapper may be needed. `std::stoi`/`stod` are convenient but throw and accept a consumed-position output; verify all input was consumed.

Never parse into a narrow type if the input could overflow before validation. Parse wide, validate, then explicitly convert.

