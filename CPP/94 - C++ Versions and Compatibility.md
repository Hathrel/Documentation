# C++ Versions and Compatibility

C++ is standardized in editions, commonly named C++11, 14, 17, 20, 23, and so on. A compiler version, language mode, and standard-library version all affect whether a feature works.

## Practical baseline

This vault uses **C++20** as its default because it provides concepts, ranges, `span`, `jthread`, chrono improvements, designated initializers, and many safety and expressiveness improvements while remaining broadly useful. Notes label important C++23 facilities such as `std::expected`, `std::print`, and `std::mdspan`.

Choose the oldest standard that satisfies your supported toolchains and dependencies. New projects with controlled current environments may choose a newer finalized standard, but verify implementation status rather than assuming every advertised feature is complete.

## Select a mode

```sh
g++ -std=c++20 ...
clang++ -std=c++20 ...
```

```powershell
cl /std:c++20 ...
```

With CMake:

```cmake
target_compile_features(my_target PUBLIC cxx_std_20)
set_target_properties(my_target PROPERTIES CXX_EXTENSIONS OFF)
```

Disabling extensions helps detect accidental dependence on compiler-specific language features.

## Feature detection

Use feature-test macros rather than compiler-version guesses:

```cpp
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
// std::expected is available at the required revision
#endif
```

Language feature macros are documented by the standard; library feature macros are available through `<version>` or the relevant header. For platform libraries and tool behavior, use build-system checks.

## Compatibility strategy

- Test every supported compiler, standard library, and OS combination in CI.
- Keep platform adaptations behind small interfaces.
- Avoid exposing unstable compiler-specific ABI across binary boundaries.
- Document fallbacks and remove them deliberately when raising the baseline.
- Check a current compiler-support table before adopting a recent feature.

“Modern C++” is a design style, not merely a flag: values, RAII, explicit ownership, standard containers and algorithms, and tooling remain useful across editions.

