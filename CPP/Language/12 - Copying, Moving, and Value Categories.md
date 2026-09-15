# Copying, Moving, and Value Categories

Copying creates an independent value. Moving transfers resources from an object that will no longer be used for its old value.

```cpp
std::string a{"large payload"};
std::string b = a;            // copy; a unchanged
std::string c = std::move(a); // move; a remains valid but unspecified
```

`std::move` does not move anything itself; it marks an expression as eligible to bind to move operations. After moving from a standard-library object, you may destroy it, assign a new value, or perform operations without preconditions; do not assume its old contents.

Copy and move are ordinary overloadable operations. A copy normally accepts `const T&` and preserves the source's value. A move normally accepts `T&&` and may transfer resources because the argument denotes an expiring object.

Moving scalar members is the same as copying them. Moving is an optimization and ownership-transfer convention, not a universal destructive primitive.

## The full value-category model

- **lvalue:** identifies a persistent object, usually has a name.
- **prvalue:** temporary/pure computed value, such as `Widget{}` or `a + b`.
- **xvalue:** expiring object, commonly produced by `std::move`.

The categories form two useful unions:

- **glvalue** (lvalue or xvalue): identifies an object or function;
- **rvalue** (prvalue or xvalue): can bind to an rvalue reference and may enable resource reuse.

An expression's category is separate from its type. Every named variable expression is an lvalue, including a variable declared `T&&`:

```cpp
void consume(Widget&& widget) {
    use(widget);              // widget is an lvalue expression
    store(std::move(widget)); // explicit xvalue
}
```

A prvalue commonly initializes its result object directly. Since C++17, many apparent temporary-to-destination moves are guaranteed not to occur.

Lvalue references `T&` bind to mutable lvalues. Rvalue references `T&&` bind to temporaries/xvalues. `const T&` can bind to both and may extend a directly bound temporary’s lifetime.

## Special member generation

The six special member functions are default constructor, destructor, copy constructor, copy assignment, move constructor, and move assignment. The compiler may implicitly declare, define, or delete them based on the class's members, bases, and user declarations.

A user-declared destructor prevents implicit move generation. Declaring a move constructor or move assignment causes implicit copy operations to be deleted. A reference or const data member can make assignment unavailable. The exact interactions are why the **rule of zero** is preferred.

Request semantics explicitly where useful:

```cpp
class Token {
public:
    Token() = default;
    Token(const Token&) = delete;
    Token& operator=(const Token&) = delete;
    Token(Token&&) noexcept = default;
    Token& operator=(Token&&) noexcept = default;
};
```

A moved-from object must satisfy the type's documented postconditions. “Valid but unspecified” means invariants hold and operations without additional preconditions are safe; it does not promise emptiness.

## Copy elision

Return local values normally:

```cpp
Widget make_widget() {
    Widget result{/*...*/};
    return result; // eligible for named return value optimization
}
```

Do not write `return std::move(result);`; it can prevent copy elision.

## Forwarding references

Generic wrapper code can preserve the caller’s value category:

```cpp
template<class T>
void relay(T&& value) {
    consume(std::forward<T>(value));
}
```

Here `T&&` is a forwarding reference because `T` is deduced. This is primarily library-author territory; ordinary code should favor clear value/reference interfaces.

If the caller passes an lvalue, `T` deduces as an lvalue-reference type and reference collapsing makes the parameter an lvalue reference. If the caller passes an rvalue, `T` is a non-reference and the parameter is an rvalue reference. `std::forward<T>` reconstructs the caller's category.

`std::forward` is appropriate only with the corresponding deduced forwarding type. Repeatedly forwarding the same argument can consume it more than once. In generic code, capture or evaluate it once when later uses depend on its value.

Mark move operations `noexcept` when they truly cannot throw; containers can then move elements during reallocation.

## Designing copy and move

For value types, copies should be independent and equality should behave unsurprisingly. For unique resource owners, delete copying and implement non-throwing moves. For polymorphic bases, direct copying can slice; provide a virtual `clone` when copying dynamic objects is part of the abstraction.

Copy assignment must handle self-assignment and preserve invariants if allocation fails. Copy-and-swap is simple and strongly exception-safe, though it may do extra work. Move assignment should also tolerate self-move unless the type explicitly documents a stronger precondition.

Do not apply `std::move` to const objects expecting an ordinary move: the result is `const T&&`, while move constructors usually require mutable `T&&` so they can alter the source. This commonly selects a copy instead.
