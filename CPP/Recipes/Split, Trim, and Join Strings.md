# Split, Trim, and Join Strings

## Trim ASCII whitespace

```cpp
std::string_view trim(std::string_view s) {
    const auto whitespace = [](unsigned char c) { return std::isspace(c) != 0; };
    while (!s.empty() && whitespace(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && whitespace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return s;
}
```

The returned view refers to the caller’s storage. The `<cctype>` function is locale-sensitive and requires conversion to `unsigned char`.

## Split on one character

```cpp
std::vector<std::string_view> split(std::string_view text, char delimiter) {
    std::vector<std::string_view> parts;
    while (true) {
        const auto pos = text.find(delimiter);
        parts.push_back(text.substr(0, pos));
        if (pos == std::string_view::npos) break;
        text.remove_prefix(pos + 1);
    }
    return parts;
}
```

This retains empty fields (`"a,,b,"`). Document that choice. The returned views dangle if the original string is destroyed or modified incompatibly.

## Join

```cpp
std::string join(std::span<const std::string> parts, std::string_view separator) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) out += separator;
        out += parts[i];
    }
    return out;
}
```

Delimiter splitting is not CSV parsing. CSV supports quoted fields, escaped quotes, and newlines; use a real parser or implement its full grammar and tests.

