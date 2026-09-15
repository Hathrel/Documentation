# Structs, Classes, and Enums

A class type defines object layout, initialization, destruction, operations, and access control. Prefer thinking in terms of invariants and value semantics rather than “bundling data with methods.”

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

Aggregates support memberwise initialization without invoking a user-declared constructor. Whether a class is an aggregate depends on detailed language rules that have evolved between standards; verify before relying on aggregate status in a public API. Designated initializers must follow member declaration order and do not provide C-style arbitrary ordering.

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

### Construction

Before a constructor body begins, the complete object is initialized in this order: virtual bases, direct bases, and then data members in declaration order. Default member initializers apply when a constructor does not explicitly initialize that member.

```cpp
class Connection {
public:
    Connection(Socket socket, Endpoint peer)
        : socket_{std::move(socket)}, peer_{std::move(peer)} {}

private:
    Socket socket_;   // initialized first
    Endpoint peer_;   // initialized second
};
```

Assignment in the constructor body is too late for references, const members, bases, and members lacking default construction; use the initializer list. Constructors can delegate to another constructor of the same class.

A single-argument constructor or conversion operator participates in implicit conversion unless marked `explicit`. C++20 permits conditional `explicit(condition)`. Keep implicit conversions rare, cheap, and unsurprising.

### Destruction and member access

Destruction reverses construction: the destructor body runs, then members and bases are destroyed in reverse order. Destructors should release owned resources and must not allow exceptions to escape.

`public`, `protected`, and `private` control name access, not memory security or runtime capability. Nested classes are separate classes for access purposes; `friend` grants a named function or class access without making it a member or creating reciprocity.

Member functions receive an implicit object argument (`this`). A const member function receives a pointer to const and can overload a non-const version. Static data and function members belong to the class scope rather than each object.

## Enums

Prefer scoped enums:

```cpp
enum class Color { red, green, blue };
Color color{Color::red};
```

They do not leak enumerator names or implicitly convert to integers. An underlying type can be specified for storage/protocol needs: `enum class Status : std::uint8_t { ... };`.

An enum's set of values is not necessarily limited to its named enumerators; casts and external data can produce other representable values. Validate values received from files, networks, or C APIs.

Unscoped enums inject their enumerator names into the surrounding scope and convert to integral types. Scoped enums avoid both behaviors. Convert deliberately with `static_cast` or C++23 `std::to_underlying`. Fixing an underlying type controls representation width but does not by itself define serialization byte order or make every integer a valid domain value.

## Special members

Classes may define or receive a destructor, copy constructor/assignment, and move constructor/assignment. Prefer the **rule of zero**: store resources in types that already manage them so no custom special members are needed. See [[Language/11 - Resource Management and RAII]] and [[Language/12 - Copying, Moving, and Value Categories]].

The compiler's implicit generation rules interact: declaring a destructor suppresses implicit move generation, and declaring move operations can delete implicit copies. Use `= default` to request normal behavior explicitly and `= delete` to reject an operation at compile time.

## Layout is not a portable wire format

Objects can contain padding for alignment. Access control, inheritance, virtual functions, and implementation ABI rules can affect layout. `sizeof` includes padding and may exceed the sum of member sizes. Standard-layout and trivially-copyable are specific type properties, not synonyms for “simple.”

Do not serialize class bytes directly or assume another compiler uses the same layout. Serialize named fields into an explicitly defined external representation.
