# Preprocessor, Macros, and Attributes

The preprocessor runs before C++ parsing. It handles file inclusion, conditional compilation, and textual macros.

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

`#` stringizes a macro argument and `##` pastes tokens. Variadic macros and `__VA_OPT__` (C++20) support controlled forwarding, but a function or template is usually safer.

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

