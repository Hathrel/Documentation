# Warnings, Sanitizers, and Static Analysis

These tools catch different defect classes and work best together.

## Sanitizers (Clang/GCC)

Debug/development example:

```sh
clang++ -std=c++20 -g -O1 -fno-omit-frame-pointer \
  -fsanitize=address,undefined main.cpp -o app
./app
```

- **AddressSanitizer (ASan):** out-of-bounds, use-after-free, some leaks.
- **UndefinedBehaviorSanitizer (UBSan):** many undefined operations.
- **ThreadSanitizer (TSan):** data races; normally use in a separate build from ASan.
- **MemorySanitizer:** uninitialized reads; chiefly Clang and requires instrumented dependencies.

MSVC supports AddressSanitizer on supported targets. Exact flags and combinations depend on compiler/platform.

Sanitizers add overhead and do not prove safety. Exercise meaningful workloads and tests. Fix the first reported error first; later reports may be consequences.

## Static analysis

`clang-tidy`, compiler analyzers, MSVC code analysis, and commercial tools inspect possible paths without executing the program. Start with a curated rule set, record suppressions with reasons, and keep third-party/generated code separate.

## Formatting

`clang-format` makes layout mechanical. Commit a `.clang-format` and format changed code in CI or pre-commit workflows. Formatting is not analysis but removes style churn from review.

## CI matrix

A strong small-project matrix includes:

- normal build with strict warnings;
- unit/integration tests;
- ASan+UBSan build;
- at least two relevant compilers/platforms;
- static analysis on changed or all first-party code.

