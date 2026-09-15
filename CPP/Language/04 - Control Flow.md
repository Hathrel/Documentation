# Control Flow

C++ control statements are familiar, but their conditions, scopes, object lifetimes, and compile-time variants have language-specific consequences. A condition accepts a value contextually convertible to bool; this includes pointers and class types with an explicit boolean conversion.

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

`case` and `default` are labels, not automatic scopes. Jumping past a declaration with initialization is ill-formed, so introduce braces when a case owns local objects. Omitting `default` for an enum can let compilers warn when a newly added enumerator is unhandled.

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

Conceptually, a range-for stores the range expression, obtains `begin` and `end` once, then iterates. `auto value` copies, `auto& value` mutates the element, `const auto& value` observes it, and `auto&& value` preserves proxy/reference behavior in generic code.

Views and iterators may borrow from the range. Prefer naming an owning intermediate when a range-producing expression could leave a view referring into a temporary.

## Scope and cleanup

Objects declared in a statement initializer or body are destroyed whenever control exits their scope, including through `break`, `continue`, `return`, and exceptions. This deterministic cleanup is the foundation of RAII.

An initializer on `if` or `switch` narrows the result's lifetime:

```cpp
if (auto result = parse(text); result) {
    use(*result);
} else {
    report(result.error());
}
```

A `goto` cannot jump into a scope past initialization of an automatic object. Jumping out destroys objects whose scopes are exited.

## Compile-time selection

`if constexpr` discards the unselected branch during template instantiation:

```cpp
template<class T>
void print_size(const T& value) {
    if constexpr (requires { value.size(); }) {
        std::cout << value.size();
    } else {
        std::cout << sizeof(value);
    }
}
```

This is compile-time selection, not merely a runtime branch optimization. Outside dependent template code, the discarded statement must still satisfy ordinary parsing and checking requirements.

Prefer a standard [[Standard Library/Algorithms and Ranges|algorithm]] when it states the operation better than loop mechanics: `std::find`, `std::sort`, `std::transform`, `std::ranges::any_of`.
