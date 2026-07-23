# Functions

Functions name behavior and create units that can be tested and reused.

```cpp
[[nodiscard]] double area(double width, double height) {
    return width * height;
}
```

The declaration contains a return type, name, parameter list, and optional qualifiers. `[[nodiscard]]` warns when callers discard a meaningful result.

## Passing arguments

```cpp
void set_count(int value);                    // small value: copy
void print(const std::string& text);           // observe existing object
void normalize(std::string& text);             // mutate caller's object
void consume(std::unique_ptr<Widget> widget);  // take ownership
```

For read-only string-like input, `std::string_view` is often better than `const std::string&`; see [[Language/07 - Strings and Text]]. Pass small, cheap types by value. Never return a reference or pointer to a local object.

## Return values

Return by value by default. Modern compilers eliminate or move most expensive copies.

```cpp
std::vector<int> make_sequence(int n) {
    std::vector<int> result;
    result.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) result.push_back(i);
    return result;
}
```

Use a small struct when returning multiple meaningful fields. Use `std::optional<T>` for “maybe a value” and exceptions or an expected-like result for failures; see [[Language/13 - Error Handling]].

## Overloading and defaults

Functions may share a name when parameter types differ. Default arguments belong in declarations and should not obscure behavior.

```cpp
void log(std::string_view message, Severity level = Severity::info);
```

Avoid overloads whose conversions make calls ambiguous. Avoid boolean parameters such as `open(file, true)`; use an enum that names the choice.

## Recursion

Recursion is natural for recursive data structures but consumes stack space. Ensure a base case and prefer iteration for simple, potentially deep loops.

