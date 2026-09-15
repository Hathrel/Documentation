# Variables, Types, and Initialization

A variable is a named object with a type, value, storage location, scope, and lifetime.

## Fundamental types

```cpp
bool enabled{true};
char grade{'A'};
int count{42};
long long large{9'000'000'000LL};
float ratio{0.5F};
double precise{3.141592653589793};
```

Integer widths vary by platform. Use `std::int32_t`, `std::uint64_t`, and related types from `<cstdint>` only when exact widths matter (file formats, protocols). `std::size_t` from `<cstddef>` is the unsigned type used for sizes and indexes.

Floating-point values approximate real numbers. Do not test results for exact equality when rounding may occur; compare within a problem-appropriate tolerance.

### Fundamental types are a family, not fixed layouts

`short`, `int`, `long`, and `long long` have minimum ranges and an ordering of minimum widths, but their exact widths are implementation-defined. Plain `char`, `signed char`, and `unsigned char` are distinct types. Whether plain `char` is signed is implementation-defined.

`sizeof(T)` reports size in bytes, and a C++ byte is `CHAR_BIT` bits. `sizeof(char)` is always 1, but that does not require eight-bit bytes. Prefer types based on meaning: ordinary `int` for routine arithmetic, `std::size_t` for object sizes, fixed-width types for exact external representations, and `std::byte` for raw bytes.

## The C++ type model

A type determines valid operations, object representation requirements, construction and destruction behavior, and overload selection. Broad categories include fundamental types, pointers, references, arrays, functions, enumerations, classes, unions, and member pointers.

Types can be modified by cv-qualification:

```cpp
int value{};
const int fixed{};       // cannot modify through this name
volatile int device{};   // observable access; not thread synchronization
```

`const` and `volatile` at the outermost level are **top-level** qualifiers. Qualifiers nested behind a pointer are **low-level** and participate differently in deduction and conversion. See [[Language/10 - Const Correctness]].

## Initialize every object

Prefer brace initialization because it rejects many narrowing conversions:

```cpp
int uninitialized;       // indeterminate local value: dangerous
int zero{};              // 0
int answer{42};
double widened{answer};
// int narrowed{3.14};    // compile error
```

Use `const` when a value should not change and `constexpr` when it can be computed at compile time:

```cpp
const int input = read_value();
constexpr double pi{3.141592653589793};
```

## Initialization is not assignment

Initialization creates an object's initial state; assignment changes an already-existing object. C++ has several syntaxes whose behavior can differ:

```cpp
Widget a;                 // default-initialization
Widget b{};               // value/list-initialization
Widget c(arg);            // direct-initialization
Widget d{arg};            // direct-list-initialization
Widget e = arg;           // copy-initialization
Widget f = {arg};         // copy-list-initialization
```

For class types, these forms select constructors. List initialization rejects narrowing and gives `std::initializer_list` constructors priority when one is viable; consequently `std::vector<int>(10, 2)` creates ten twos while `std::vector<int>{10, 2}` creates two elements.

Default-initialization does not necessarily zero an object. A local fundamental value such as `int n;` has an indeterminate value; reading it is invalid except in narrow language-defined cases. Value-initialization with `{}` generally supplies a zero or empty starting state.

Beware the **most vexing parse**:

```cpp
Widget item(); // declares a function, not an object
Widget item{}; // definitely an object
```

### Object initialization order

For a class, virtual bases are initialized first, then direct bases, then data members in declaration order, then the constructor body runs. The written order of a member-initializer list does not change this. Initialize members in declaration order and enable reorder warnings.

## Type inference

`auto` asks the compiler to infer a type. Use it when the type is obvious from the initializer or verbose—not to hide important semantics.

```cpp
auto retries = 3;                    // int
auto name = std::string{"Ada"};     // std::string
const auto size = name.size();       // std::size_t
```

Plain `auto` drops top-level `const` and references. Use `const auto&` to observe without copying and `auto&` to mutate an existing object.

### `auto` and `decltype`

`auto` mostly follows template argument deduction:

```cpp
const int n{3};
auto a = n;         // int
auto& b = n;        // const int&
auto&& c = n;       // const int& through reference collapsing
const auto* p = &n; // pointer to const int
```

`decltype(name)` yields the declared type of an unparenthesized name. For other expressions, `decltype((expression))` reflects the expression's value category and may produce a reference:

```cpp
int x{};
decltype(x) a{};    // int
decltype((x)) b=x;  // int&, because (x) is an lvalue
```

`decltype(auto)` preserves these rules and is useful in forwarding code, but can accidentally return a dangling reference. Use it only when exact type preservation is intended.

## Scope and lifetime

A name declared inside `{}` is visible from its declaration to the block’s end. Its automatic object is destroyed at block exit. Prefer the smallest useful scope.

Names can have block, function, class, namespace, or template-parameter scope. Inner declarations can hide outer names; shadowing warnings catch many mistakes. An object's storage duration can be automatic, static, thread, or dynamic and is distinct from the visibility of its name.

Type aliases name types without creating new types:

```cpp
using UserId = std::uint64_t; // still exactly uint64_t
```

Use a small wrapper class when two values need to be non-interchangeable despite sharing a representation.

Type conversions and casts are covered in [[Language/03 - Expressions and Operators]]. Ownership and lifetime are covered in [[Language/06 - References, Pointers, and Lifetimes]].
