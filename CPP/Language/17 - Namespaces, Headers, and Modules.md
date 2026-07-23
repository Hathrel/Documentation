# Namespaces, Headers, and Modules

## Namespaces

Namespaces prevent collisions and organize APIs.

```cpp
namespace acme::math {
    int clamp(int value, int low, int high);
}
```

Do not write `using namespace ...` in a header. In a source file or narrow function scope, a selective `using std::string;` can be reasonable. Unnamed namespaces in `.cpp` files give names internal linkage:

```cpp
namespace {
int helper() { return 42; }
}
```

## Headers

A header should be self-contained: including it first in an empty translation unit should compile. Use `#pragma once` (widely supported) or include guards. Put declarations and types in headers; put non-template definitions in source files.

```cpp
// widget.hpp
#pragma once
#include <string>

namespace app {
class Widget {
public:
    explicit Widget(std::string name);
    [[nodiscard]] const std::string& name() const noexcept;
private:
    std::string name_;
};
}
```

Forward declarations can reduce coupling when only a pointer/reference appears, but complete types are needed for objects by value, inheritance, and most inline usage. Prefer correctness and clarity over aggressive forward declaration.

## Linkage and `inline`

`inline` permits identical definitions in multiple translation units; it does not promise machine-code inlining. Functions defined inside a class body are implicitly inline. C++17 inline variables allow header-defined shared constants: `inline constexpr int version = 1;`.

## Modules (C++20)

Modules replace textual inclusion with explicit import/export boundaries:

```cpp
// math.cppm
export module math;
export int add(int a, int b) { return a + b; }

// consumer
import math;
```

Compiler and build-system module workflows still vary. Learn headers first; adopt modules when your toolchain provides a stable supported workflow.

---
Links: [[Notes on Headers]]

