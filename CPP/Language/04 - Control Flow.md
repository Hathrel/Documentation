# Control Flow

## Conditions

```cpp
if (temperature < 0) {
    status = "freezing";
} else if (temperature < 20) {
    status = "cool";
} else {
    status = "warm";
}
```

An initializer can limit a temporary’s scope:

```cpp
if (const auto it = scores.find(name); it != scores.end()) {
    std::cout << it->second;
}
```

Use `switch` for discrete integral or enum cases. Add `break`, `return`, or `[[fallthrough]]` deliberately.

```cpp
switch (command) {
case Command::start: run(); break;
case Command::stop:  halt(); break;
default:              report_unknown(); break;
}
```

## Loops

```cpp
for (int i = 0; i < 10; ++i) { /* counted */ }
while (read_next()) { /* condition-driven */ }
do { /* executes at least once */ } while (retry());
```

Prefer a range-based loop for sequences:

```cpp
for (const auto& item : items) {
    print(item);              // no copy; cannot mutate item
}
for (auto& item : items) {
    normalize(item);          // mutates elements
}
```

`break` exits the nearest loop/switch; `continue` starts the next loop iteration; `return` exits the current function.

Prefer a standard [[Standard Library/Algorithms and Ranges|algorithm]] when it states the operation better than loop mechanics: `std::find`, `std::sort`, `std::transform`, `std::ranges::any_of`.

