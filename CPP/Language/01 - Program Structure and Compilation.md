# Program Structure and Compilation

## Translation pipeline

```text
source (.cpp) -> preprocessing -> compilation -> object file -> linking -> executable
```

1. The **preprocessor** handles directives such as `#include` and conditional compilation.
2. The **compiler** parses and type-checks each translation unit, then produces object code.
3. The **linker** combines object files and libraries and resolves cross-file symbols.

A **translation unit** is one source file after its included headers have been expanded.

## Declarations and definitions

A declaration tells the compiler a name and type. A definition creates the function, object, or class body.

```cpp
int add(int, int);              // declaration
int add(int a, int b) {         // definition
    return a + b;
}
```

Every used non-inline function or object needs exactly one program-wide definition: the **One Definition Rule** (ODR). Classes, templates, and inline entities may be defined identically in multiple translation units, usually through headers.

## Multiple files

`math.hpp`:

```cpp
#pragma once
int add(int a, int b);
```

`math.cpp`:

```cpp
#include "math.hpp"
int add(int a, int b) { return a + b; }
```

`main.cpp`:

```cpp
#include "math.hpp"
#include <iostream>

int main() {
    std::cout << add(2, 3) << '\n';
}
```

Compile both source files: `g++ -std=c++20 main.cpp math.cpp -o app`.

Use angle brackets for system/third-party headers and quotes for project headers. Include what you use; do not rely on another header including something indirectly.

See [[Language/17 - Namespaces, Headers, and Modules]] and [[Engineering/CMake and Project Structure]].

