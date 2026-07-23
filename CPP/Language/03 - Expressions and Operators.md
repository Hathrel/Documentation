# Expressions and Operators

Expressions compute values and may have side effects.

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

## Bit operations

Use unsigned integer types for bit masks. Prefer `std::bitset`, scoped enums with explicit helpers, or `<bit>` utilities when they express the intent. Never confuse bitwise `&` with logical `&&`.

