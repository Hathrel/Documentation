# Functions

Functions name behavior and create units that can be tested and reused.

```cpp
[[nodiscard]] double area(double width, double height) {
    return width * height;
}
```

The declaration contains a return type, name, parameter list, and optional qualifiers. `[[nodiscard]]` warns when callers discard a meaningful result.

## Declarations, definitions, and function types

A declaration introduces the callable interface; a definition supplies its body. Parameter names are optional in a declaration. Return type, parameter-type list, cv/ref qualifiers for member functions, and exception specification all contribute to how a function can be called. The return type alone cannot distinguish overloads.

```cpp
int parse(std::string_view text);            // declaration
int parse(std::string_view text) { /*...*/ } // definition

using Predicate = bool(const Record&);       // function type
Predicate* choose_filter();
```

Function names often decay to function pointers. Prefer lambdas or templates for generic callbacks; use `std::function` when you specifically need owning, type-erased callable storage.

Arguments are evaluated before entering the function, but their relative evaluation order is generally unspecified. Do not make one argument depend on another argument's side effect.

## Passing arguments

```cpp
void set_count(int value);                    // small value: copy
void print(const std::string& text);           // observe existing object
void normalize(std::string& text);             // mutate caller's object
void consume(std::unique_ptr<Widget> widget);  // take ownership
```

For read-only string-like input, `std::string_view` is often better than `const std::string&`; see [[Language/07 - Strings and Text]]. Pass small, cheap types by value. Never return a reference or pointer to a local object.

Parameter declarations encode a contract:

- `T` receives a separate value; copy or move it into the parameter.
- `const T&` borrows an existing object for read-only access.
- `T&` borrows it for mutation and cannot bind to an ordinary temporary.
- `T*` conventionally means an optional or array-like non-owning input; document nullability.
- `T&&` in a non-template function usually means the function may consume the argument.
- `T&&` with a deduced `T` is a forwarding reference and follows different rules.

Passing by value is often best when the function needs its own copy anyway:

```cpp
class Person {
public:
    explicit Person(std::string name) : name_{std::move(name)} {}
private:
    std::string name_;
};
```

An lvalue caller pays one copy and then the parameter is moved into the member; an rvalue caller can move twice cheaply. For small scalar and iterator-like types, by-value is simpler than indirection.

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

### Copy elision and references

Returning a local class object by value is the normal efficient pattern. Named return value optimization may construct it directly in the caller. Returning a matching temporary prvalue has guaranteed copy elision since C++17.

Do not write `return std::move(local);` merely to “help”; it can prevent named return value optimization. Return a reference or pointer only when the function is intentionally exposing an object that outlives the call. A reference to a local variable or by-value parameter dangles immediately.

`auto` return-type deduction drops references like ordinary `auto`. `decltype(auto)` preserves them and therefore requires careful lifetime analysis.

## Overloading and defaults

Functions may share a name when parameter types differ. Default arguments belong in declarations and should not obscure behavior.

```cpp
void log(std::string_view message, Severity level = Severity::info);
```

Avoid overloads whose conversions make calls ambiguous. Avoid boolean parameters such as `open(file, true)`; use an enum that names the choice.

### How overload resolution thinks

For a call, the compiler:

1. collects candidate functions found by ordinary lookup and argument-dependent lookup;
2. removes candidates whose parameter counts, constraints, or conversions are not viable;
3. ranks the implicit conversion sequence for each argument;
4. applies tie-breakers for templates, constraints, and other language rules.

Exact matches generally beat promotions, which beat other standard conversions, which beat user-defined conversions. Default arguments do not make a conversion better. `nullptr` selects pointer overloads more reliably than `0`, which is also an integer.

Argument-dependent lookup also searches namespaces associated with argument types. It enables unqualified customization patterns such as `swap(a, b)` but can reveal overloads that were not obvious from the call site.

Default arguments are substituted at the call site and are not dynamically dispatched. Put a default in one visible declaration, normally a header. Adding a default can change source behavior without changing a function's binary symbol.

## Member functions

A non-static member function has an implicit object parameter, exposed as `this`. Qualifiers constrain that object:

```cpp
class Buffer {
public:
    std::span<std::byte> bytes() &;
    std::span<const std::byte> bytes() const &;
    std::span<const std::byte> bytes() && = delete;
};
```

The cv-qualifier distinguishes mutable and const objects; a ref-qualifier can distinguish lvalue and rvalue objects. A `static` member has no `this` pointer. A `virtual` member dispatches using the dynamic type when called through a base pointer or reference.

## `noexcept`

An exception specification is part of a function's type. If an exception escapes a `noexcept` function, `std::terminate` is called. Destructors are implicitly non-throwing in common cases, and standard containers often prefer a type's move operation only when it is non-throwing.

Use `noexcept` when the implementation and everything it calls truly uphold that contract. Conditional specifications support generic code:

```cpp
template<class T>
void exchange(T& a, T& b)
    noexcept(std::is_nothrow_move_constructible_v<T>);
```

## Recursion

Recursion is natural for recursive data structures but consumes stack space. Ensure a base case and prefer iteration for simple, potentially deep loops.
