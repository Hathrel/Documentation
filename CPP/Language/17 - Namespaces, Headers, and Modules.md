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

A header is a reusable source fragment, normally containing the declarations that several translation units need to share. `#include` is textual: before compilation, the preprocessor replaces the directive with the contents of the named file. The compiler does not compile a header separately in the usual model.

Header names conventionally end in `.h`, `.hpp`, `.hh`, or `.hxx`; C++ assigns no special meaning to these extensions. A header also does not have to be beside its matching `.cpp` file. The build system's include paths determine where the preprocessor searches.

### What belongs in a header

Put an entity in a header when another translation unit needs to know about it:

- function declarations;
- class, struct, enum, alias, and concept definitions;
- template definitions;
- `inline` function and variable definitions;
- constants intended as part of the interface.

Ordinary non-`inline` function definitions and storage for non-`inline` global variables normally belong in one `.cpp` file. Placing either in a header included by several `.cpp` files usually violates the One Definition Rule (ODR) and produces linker errors.

Not every `.cpp` file needs a matching header. A header is useful when declarations must be shared, not merely because a source file exists. Helpers used by only one `.cpp` file can remain private to that file, often in an unnamed namespace.

### Header/source split

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

```cpp
// widget.cpp
#include "widget.hpp" // Include the matching header first.

#include <utility>

namespace app {
Widget::Widget(std::string name) : name_{std::move(name)} {}

const std::string& Widget::name() const noexcept {
    return name_;
}
}
```

A function can be defined before its first use in one `.cpp` file without a header. The separate header exists so `main.cpp`, tests, and other clients can all see the same declaration while `widget.cpp` supplies the single definition.

### Self-contained headers and include-what-you-use

A header should compile when included first in an otherwise empty translation unit. It must directly include the headers needed for its own declarations rather than relying on a previous or transitive include.

```cpp
// report.hpp
#pragma once

#include <string>       // std::string is a data member.
#include <string_view>  // std::string_view appears in the interface.

class Report {
public:
    explicit Report(std::string title);
    bool contains(std::string_view text) const;

private:
    std::string title_;
};
```

Transitive includes are implementation details and can disappear after an otherwise harmless library update. "Include what you use" makes dependencies explicit and headers resistant to include-order bugs.

In a `.cpp` file, include its matching header first. This is a simple way to expose missing dependencies in that header. Then include other project and standard-library headers in a consistent order.

### Preventing repeated inclusion

The same header can be reached through several include paths, so protect it against repeated inclusion. `#pragma once` is concise and supported by major compilers:

```cpp
#pragma once
```

Traditional include guards are standard and fully portable:

```cpp
#ifndef ACME_WIDGET_HPP
#define ACME_WIDGET_HPP

// Header contents

#endif // ACME_WIDGET_HPP
```

Guard names must be unique across the program. Use a project-specific prefix and the file path rather than a generic name such as `WIDGET_H`.

### Quotes, angle brackets, and include paths

Use quotes for project headers and angle brackets for standard-library or installed dependency headers:

```cpp
#include "acme/widget.hpp"
#include <string>
#include <third_party/library.hpp>
```

The exact search order is implementation-defined and configured by compiler options such as `-I` or by build-system usage requirements such as CMake's `target_include_directories`. Consumers of a library should include from its public include root (`"acme/widget.hpp"`), not through relative paths such as `"../../include/acme/widget.hpp"`.

### Forward declarations and complete types

A forward declaration can avoid including another project header when only the existence of a type is required:

```cpp
#pragma once

#include <memory>

namespace app {
class Renderer;

class Window {
public:
    explicit Window(Renderer& renderer);
    ~Window(); // Define in the .cpp where Renderer is complete.

private:
    Renderer& renderer_;
    std::unique_ptr<Renderer> owned_renderer_;
};
}
```

Include the defining header when the complete type is required—for example, for a data member stored by value, inheritance, most inline operations on the type, or templates whose implementation needs its members. Prefer a direct include when in doubt; aggressive forward declarations can make interfaces fragile. Do not forward-declare standard-library types yourself.

Forward declarations also help break circular includes. If two classes only point or refer to each other, forward-declare each type in the opposite header and include both headers in the `.cpp` files. If both must contain each other by value, the design itself is impossible because each object would require the other to have a completed size.

### Definitions that legitimately live in headers

Template definitions normally belong in headers because the compiler must see their definitions when instantiating them. Definitions inside a class body are implicitly `inline`; definitions outside it must be marked `inline` if the header can appear in multiple translation units.

```cpp
template<class T>
T twice(T value) {
    return value + value;
}

inline int next_id() {
    static int id = 0;
    return ++id;
}

inline constexpr int protocol_version = 3; // C++17
```

Here `inline` is primarily an ODR property: it permits identical definitions in multiple translation units. It does not require the compiler to substitute the function body at each call. Variables declared `constexpr` at namespace scope have special linkage rules; `inline constexpr` clearly expresses one shared header-defined constant when identity matters.

### Public and private headers

Installed library headers form a public API: changing them can force consumers to recompile and can break source or binary compatibility. Keep implementation details in `.cpp` files or non-installed private headers. The PImpl idiom goes further by storing a pointer to a forward-declared implementation type, trading an allocation and indirection for a smaller, more stable public header.

### Common header problems

- Defining a non-`inline` function or global variable in a widely included header.
- Depending on transitive includes or a particular include order.
- Writing `using namespace ...` at header scope, which changes every includer's namespace lookup.
- Giving internal macros short, generic names that leak into includers.
- Creating circular includes instead of separating interfaces or using forward declarations.
- Putting large implementation details in public headers, increasing compile times and coupling.
- Including platform-heavy headers in a public interface when a smaller abstraction would suffice.

Useful checks include compiling a tiny file that includes only the header, enabling compiler warnings, and building with unity/jumbo compilation both disabled and enabled where the project supports it. Tools commonly called *include-what-you-use* analyzers can suggest missing and unnecessary direct includes, but their results still require judgment.

## Linkage and `inline`

Names declared at namespace scope have linkage rules that determine whether declarations in different translation units denote the same entity. Header design and the ODR therefore meet at `inline`, `constexpr`, templates, and global objects. See the header-specific examples above and [[Language/01 - Program Structure and Compilation|Program Structure and Compilation]] for the translation pipeline.

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
