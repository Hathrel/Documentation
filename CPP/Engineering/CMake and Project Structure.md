# CMake and Project Structure

CMake generates native build files and describes targets, dependencies, and usage requirements.

## Small project

```text
my_project/
├── CMakeLists.txt
├── include/my_project/calculator.hpp
├── src/calculator.cpp
├── app/main.cpp
└── tests/calculator_test.cpp
```

```cmake
cmake_minimum_required(VERSION 3.25)
project(my_project VERSION 0.1.0 LANGUAGES CXX)

add_library(calculator src/calculator.cpp)
target_include_directories(calculator PUBLIC include)
target_compile_features(calculator PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(calculator PRIVATE /W4 /permissive-)
else()
  target_compile_options(calculator PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()

add_executable(app app/main.cpp)
target_link_libraries(app PRIVATE calculator)
```

Configure and build:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

## Target-based CMake

Attach properties to targets with `target_*` commands. `PRIVATE` affects only the target, `PUBLIC` affects target and consumers, `INTERFACE` affects only consumers. Prefer imported targets such as `SomePackage::Library` over global include/link variables.

Do not set global compiler flags or hardcode platform-specific output paths unnecessarily. Use `target_compile_features` rather than manually adding `-std=`.

## Tests

```cmake
include(CTest)
if(BUILD_TESTING)
  add_executable(calculator_tests tests/calculator_test.cpp)
  target_link_libraries(calculator_tests PRIVATE calculator)
  add_test(NAME calculator_tests COMMAND calculator_tests)
endif()
```

Run with `ctest --test-dir build --output-on-failure -C Debug`.

For repeatable local/CI configurations, use `CMakePresets.json`. Keep generated `build/` directories out of source control.

