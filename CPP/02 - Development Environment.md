# Development Environment

A C++ program starts as source text and becomes a native executable through a compiler toolchain.

## Choose a toolchain

- **Windows:** Visual Studio 2022 Build Tools or Visual Studio Community (MSVC); Clang is also available.
- **macOS:** Xcode Command Line Tools (Apple Clang).
- **Linux:** GCC (`g++`) or Clang (`clang++`).

Use a recent compiler with C++20 support. An editor such as Visual Studio, VS Code with C/C++, CLion, or another language-server-capable editor provides completion and diagnostics.

## Verify the compiler

```sh
g++ --version
clang++ --version
cl
```

Only one needs to work. `cl` must be run from a Visual Studio Developer PowerShell/Command Prompt.

## Compile one file

GCC or Clang:

```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g main.cpp -o app
./app
```

MSVC:

```powershell
cl /std:c++20 /W4 /permissive- /EHsc /Zi main.cpp /Fe:app.exe
.\app.exe
```

Flags select C++20, enable warnings, and retain debug information. Warnings are clues, not decoration—fix them or document why they are safe.

## Debug and release builds

- Debug: fast compilation, assertions and debug symbols, little optimization.
- Release: optimization enabled; behavior must remain correct.

Typical GCC/Clang additions are `-O0` for debug and `-O2 -DNDEBUG` for release. With real projects, let [[Engineering/CMake and Project Structure|CMake]] choose these through build configurations.

## Useful tools

- **Debugger:** Visual Studio Debugger, GDB, or LLDB
- **Build system:** CMake
- **Formatter:** `clang-format`
- **Static analyzer:** `clang-tidy`
- **Dynamic checking:** AddressSanitizer and UndefinedBehaviorSanitizer
- **Version control:** Git

See [[Engineering/Debugging]], [[Engineering/Warnings, Sanitizers, and Static Analysis]], and [[Engineering/Dependencies and Package Managers]].

