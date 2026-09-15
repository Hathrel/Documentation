# Expressions and Operators

Expressions compute values and may have side effects.

Every expression has a type and a **value category**. The category describes how the result relates to an object, which determines reference binding, overload selection, and whether moving is possible:

- an **lvalue** identifies an object with persistent identity;
- an **xvalue** identifies an object whose resources may be reused;
- a **prvalue** computes or initializes a value;
- **glvalue** means lvalue or xvalue; **rvalue** means prvalue or xvalue.

A named variable is an lvalue even when its type is `T&&`. `std::move(x)` does not move anything by itself; it casts `x` to an xvalue so a later operation may select move behavior. See [[Language/12 - Copying, Moving, and Value Categories]].

## Common operators

```cpp
int sum = a + b;
int remainder = a % b;
bool in_range = x >= 0 && x < 10;
++x;             // increment
total += value;  // compound assignment
```

Categories include arithmetic (`+ - * / %`), comparison (`== != < <= > >=`), logical (`! && ||`), assignment, bitwise (`~ & | ^ << >>`), member access (`.` and `->`), call (`()`), and indexing (`[]`).

Integer division truncates: `7 / 2` is `3`; `7.0 / 2` is `3.5`. Signed integer overflow is undefined behavior. Unsigned arithmetic wraps modulo 2^N, but unsigned values are still a poor choice for ordinary arithmetic merely to disallow negatives.

`&&` and `||` short-circuit from left to right:

```cpp
if (ptr != nullptr && ptr->ready()) { /* safe dereference */ }
```

## Precedence

Do not memorize the entire precedence table. Parenthesize mixed expressions so intent is visible:

```cpp
const bool accepted = is_admin || (active && has_permission);
```

Avoid modifying the same object multiple times in one expression. Keep side effects in clear statements.

## Evaluation order and sequencing

Precedence controls grouping, not runtime evaluation order. Associativity says how operators group; neither generally tells you which operand is evaluated first. C++ does guarantee sequencing in specific places, including left-to-right for `&&`, `||`, the comma operator, and the first operand of `?:`.

```cpp
f(i++, i++); // arguments do not have a portable relative evaluation order
```

Since C++17 the two argument evaluations are indeterminately sequenced rather than interleaved, but which happens first is still unspecified. Split state changes into statements whenever order matters.

## Standard conversions

Implicit conversion sequences drive initialization, arithmetic, conditions, calls, and overload resolution. Important groups include:

- lvalue-to-rvalue conversion, which reads an object's stored value;
- array-to-pointer and function-to-pointer conversion;
- integral and floating-point promotions;
- numeric conversions between widths or signedness;
- qualification conversion, such as `int*` to `const int*`;
- pointer conversion to a base class or to `void*`;
- user-defined conversions through converting constructors or conversion operators.

Promotions are favored over other conversions during overload resolution. Mixed arithmetic first applies the **usual arithmetic conversions** to find a common type. Signed/unsigned mixing is a frequent source of surprises:

```cpp
int debt{-1};
std::size_t count{2};
// debt < count may be false after converting debt to an unsigned type
```

Do not silence such warnings mechanically. Align the domain types, validate before converting, or use `std::cmp_less` from `<utility>`.

Contextual conversion to `bool` is used by conditions and logical operators. Pointers test non-nullness, integers test nonzero, and classes may provide `explicit operator bool()`.

## Conversions and casts

Implicit conversions can lose data. Enable conversion warnings and be explicit where intent matters:

```cpp
const auto length = static_cast<double>(items.size());
```

- `static_cast<T>`: checked-at-compile-time numeric and related conversions.
- `dynamic_cast<T>`: checked polymorphic downcast; often a design smell.
- `const_cast<T>`: adjusts constness; rarely appropriate.
- `reinterpret_cast<T>`: low-level representation conversion; expert-only.

Avoid C-style casts because they can silently perform several different operations.

`static_cast` expresses a permitted language conversion but does not make a lossy numeric conversion safe. A downcast with `static_cast<Derived&>(base)` has undefined behavior if the object is not actually a `Derived`. `dynamic_cast` checks polymorphic relationships at runtime, returning null for a failed pointer cast or throwing `std::bad_cast` for a failed reference cast.

`const_cast` may remove constness from an access path, but modifying an object originally defined `const` is undefined behavior. `reinterpret_cast` does not generally create an object of the destination type or make aliasing valid.

## Operator overloading

At least one operand of an overloaded operator must have class or enum type. Overloading cannot change precedence, arity, or built-in short-circuit rules; overloaded `&&` and `||` behave like function calls and therefore should almost never be used.

Prefer operators when the meaning is conventional and unsurprising. Implement compound assignment as the core mutating operation and derive the binary operator from it:

```cpp
Counter& Counter::operator+=(int amount) {
    value_ += amount;
    return *this;
}

Counter operator+(Counter lhs, int rhs) {
    lhs += rhs;
    return lhs;
}
```

Comparison can often use a defaulted `operator==` and, in C++20, `operator<=>`. Operator overloads should preserve expected algebraic and value semantics.

## Temporary materialization

Prvalues often initialize their destination directly due to guaranteed copy elision. When a glvalue is required, a temporary object is materialized. Temporaries normally live to the end of the full expression, with specific lifetime-extension rules for references. Do not assume that storing a pointer, iterator, or view into a temporary extends the temporary's lifetime.

## Bit operations

Use unsigned integer types for bit masks. Prefer `std::bitset`, scoped enums with explicit helpers, or `<bit>` utilities when they express the intent. Never confuse bitwise `&` with logical `&&`.
