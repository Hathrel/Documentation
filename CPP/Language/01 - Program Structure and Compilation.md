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

### Not every declaration is a definition

These distinctions matter because the compiler processes translation units independently:

```cpp
extern int request_count;       // declaration, storage defined elsewhere
int request_count{};            // definition

class Parser;                   // declaration of an incomplete type
class Parser {};                // definition of the type

int parse(std::string_view);    // function declaration
```

A class definition does not create a class object, but it is still a definition because it completely defines the type. A function declaration may appear in many translation units; one non-`inline` definition is then linked to all calls.

## Translation units and separate compilation

After preprocessing, each `.cpp` plus all text included into it forms one translation unit. Names and types visible in one translation unit are not automatically visible in another. This explains several characteristic C++ errors:

- a compiler error usually means the current translation unit lacks a declaration or violates a language rule;
- an “undefined reference” or “unresolved external” usually means a declaration was compiled but no matching definition reached the linker;
- a “multiple definition” error usually means a non-`inline` definition appeared in more than one translation unit;
- an ABI mismatch can compile and link yet fail at runtime when separately built code disagrees about layout or calling conventions.

The declaration seen by callers and the definition compiled elsewhere must describe the same entity. Include the declaring header in the defining `.cpp` so the compiler checks that agreement.

## Linkage, scope, and storage duration

These are separate properties:

- **scope** controls where a name can be written;
- **linkage** controls whether declarations in different scopes or translation units name the same entity;
- **storage duration** controls how long an object's storage exists;
- **lifetime** is when an object actually exists in that storage.

Namespace-scope functions and non-`const` variables normally have external linkage. An unnamed namespace gives its contents internal linkage, producing a separate entity per translation unit:

```cpp
namespace {
int next_token() { /* implementation-only */ }
}
```

Block-local objects normally have automatic storage duration. Namespace-scope objects, static data members, and function-local `static` objects have static storage duration. `thread_local` creates one object per thread. Dynamically allocated storage lasts until explicitly released, but the lifetime of an object within storage can begin or end separately in low-level code.

## The ODR in practice

The ODR has two important patterns:

1. Ordinary functions and objects that are odr-used require one definition in the entire program.
2. Types, templates, and `inline` entities may be defined in multiple translation units when every definition satisfies the ODR—normally because the same header supplied identical token sequences.

Do not put an ordinary global definition such as `int count{};` in a shared header. Use `extern int count;` in the header and one definition in a source file, or use a C++17 `inline` variable when a header definition is intentional.

“No diagnostic required” appears in some ODR rules: an invalid program is not guaranteed to be rejected because no single compiler invocation sees the whole program. Avoid conditional macros that make a class or inline definition differ between translation units.

## Static initialization

Objects with static storage duration are zero-initialized first, then constant-initialized when possible, otherwise dynamically initialized. The relative dynamic-initialization order of objects in different translation units is often unspecified—the **static initialization order fiasco**.

Prefer constant initialization, avoid namespace-scope objects with interdependent constructors, or use a function-local static:

```cpp
Registry& registry() {
    static Registry instance; // initialized on first call; thread-safe since C++11
    return instance;
}
```

Destruction order is the reverse of completed initialization and can create a similar shutdown hazard.

## Compile, link, and ABI boundaries

Compilation flags that affect class layout, exception handling, runtime-library choice, packing, or language mode must agree across binary boundaries. The C++ standard specifies source-language behavior, not a universal binary ABI. Libraries built with incompatible compilers, settings, or standard-library implementations may not be safely interchangeable even when declarations look identical.

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
