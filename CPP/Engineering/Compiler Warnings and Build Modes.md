# Compiler Warnings and Build Modes

## Suggested warnings

GCC/Clang starting point:

```text
-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
```

MSVC starting point:

```text
/W4 /permissive- /EHsc
```

Add warnings per toolchain and project; some are noisy for generated/third-party headers. Do not globally force your warning policy onto downstream consumers of a library.

Treat warnings as defects. In CI, `-Werror` or `/WX` prevents regressions, but pin toolchain versions or manage upgrades because new compilers introduce warnings.

## Configurations

- **Debug:** symbols, assertions, minimal optimization; easiest debugging.
- **Release:** optimization and often `NDEBUG`; production performance.
- **RelWithDebInfo:** optimized with symbols; useful for profiling and production diagnostics.
- **MinSizeRel:** optimize primarily for size.

Never make correctness depend on configuration. `assert` disappears with `NDEBUG`, so it cannot perform required validation or side effects.

## Optimization

Optimized debugging can show variables as unavailable and reorder/eliminate statements. Reproduce logic in Debug when possible, but also test Release: undefined behavior and timing bugs often surface only under optimization.

## Compatibility

Select a language standard explicitly and test the compilers/platforms you support. `__cplusplus` and feature-test macros in `<version>` can guard optional standard features, but prefer build-system capability checks for platform libraries.

Keep build output and generated files outside the source tree where practical.

