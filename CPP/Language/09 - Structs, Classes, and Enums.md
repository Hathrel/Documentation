# Structs, Classes, and Enums

## Aggregates and structs

Use a `struct` for a simple value with public data and invariant-free fields:

```cpp
struct Point {
    double x{};
    double y{};
};

Point origin{};
Point p{.x = 2.0, .y = 3.0}; // designated initialization, C++20
```

## Classes and invariants

A class can ensure its state is always valid.

```cpp
class BankAccount {
public:
    explicit BankAccount(long long cents) : cents_{cents} {
        if (cents < 0) throw std::invalid_argument{"negative balance"};
    }

    [[nodiscard]] long long balance_cents() const { return cents_; }
    void deposit(long long cents);

private:
    long long cents_{};
};
```

`class` defaults to private access; `struct` defaults to public. That is the only language-level difference. Constructors establish invariants. `explicit` prevents surprising implicit conversion from the constructor argument. Member initializer lists initialize members before the constructor body; members are initialized in declaration order.

Do not write getters and setters mechanically. Expose operations that preserve the abstraction (`deposit`) instead of unrestricted state mutation (`set_balance`).

## Enums

Prefer scoped enums:

```cpp
enum class Color { red, green, blue };
Color color{Color::red};
```

They do not leak enumerator names or implicitly convert to integers. An underlying type can be specified for storage/protocol needs: `enum class Status : std::uint8_t { ... };`.

## Special members

Classes may define or receive a destructor, copy constructor/assignment, and move constructor/assignment. Prefer the **rule of zero**: store resources in types that already manage them so no custom special members are needed. See [[Language/11 - Resource Management and RAII]] and [[Language/12 - Copying, Moving, and Value Categories]].

