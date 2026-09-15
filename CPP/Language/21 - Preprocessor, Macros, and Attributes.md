# Preprocessor, Macros, and Attributes

The preprocessor runs before C++ parsing. It handles file inclusion, conditional compilation, and textual macros.

Preprocessing works with tokens rather than C++ types, scopes, overloads, or object lifetimes. Its output becomes the token stream compiled as C++. A macro can therefore change code far from its definition and can affect parsing in ways no C++ name could.

## Directives and source control

Directives begin with `#` after optional whitespace. Important directives include `#include`, `#define`, `#undef`, `#if`/`#elif`/`#else`/`#endif`, `#error`, and implementation-specific `#pragma`.

Conditional compilation removes tokens before parsing. Both branches should still be built regularly on their target platforms; an inactive branch receives no ordinary type checking.

## Conditional compilation

```cpp
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#else
#error Unsupported platform
#endif
```

Build-system-generated configuration macros are preferable for detected library capabilities. Keep platform branches at narrow integration seams.

## Macros

Macros do not obey C++ scope or type rules, and arguments can be evaluated unexpectedly:

```cpp
#define BAD_SQUARE(x) ((x) * (x)) // BAD_SQUARE(i++) modifies i twice
```

Prefer:

```cpp
template<class T>
constexpr T square(T x) { return x * x; }
```

Use macros when only the preprocessor can do the job: include guards, conditional compilation, controlled code generation, or integration with a macro API. Parenthesize expression macro parameters and results, avoid repeated evaluation, use distinctive project prefixes, and never let public macros have generic names.

Function-like macro arguments are expanded textually and can be evaluated zero, one, or several times depending on the replacement. Parentheses protect precedence but cannot repair repeated side effects, lost scope, surprising control flow, or type blindness.

Multi-statement macros traditionally use a `do { ... } while (false)` wrapper so they behave as one statement. This is still a compatibility technique; an inline function is preferable whenever typed C++ can express the job.

Macro replacement is suppressed or altered around stringizing and token pasting, which sometimes requires two expansion layers. Treat such metaprogramming as an isolated implementation detail with focused tests.

`#` stringizes a macro argument and `##` pastes tokens. Variadic macros and `__VA_OPT__` (C++20) support controlled forwarding, but a function or template is usually safer.

## Feature detection

Prefer standard feature-test macros such as language `__cpp_*` and library `__cpp_lib_*` macros over compiler-version guesses. Include `<version>` when testing library features unless the feature's normal header already supplies its macro.

Platform macros describe the build environment, not necessarily API availability. Let the build system test headers, symbols, and behavior, then generate narrowly named configuration macros.

## Predefined information

`__FILE__` and `__LINE__` exist, but `std::source_location` (C++20) captures caller context through ordinary typed interfaces. Standard feature-test macros are safer than compiler-version arithmetic.

## Attributes

Attributes attach standardized or implementation-specific metadata:

```cpp
[[nodiscard("check the error")]] Result do_work();
[[maybe_unused]] const auto debug_value = compute();
[[deprecated("use new_api")]] void old_api();
[[fallthrough]];
```

Other standard attributes include `noreturn`, `likely`, `unlikely`, and `no_unique_address`. Attributes do not replace correct control flow or profiling. Pragmas and vendor attributes are implementation-specific; isolate and document them.

An implementation may ignore an unknown standard attribute when the grammar permits, but vendor attributes have vendor-defined behavior. `[[likely]]` and `[[unlikely]]` express an expectation, not a branch guarantee, and can hurt when guessed incorrectly. `[[no_unique_address]]` permits overlapping storage for a potentially empty member but does not promise a specific layout.
