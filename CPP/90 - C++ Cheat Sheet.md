# C++ Cheat Sheet

## Build

```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g main.cpp -o app
clang++ -std=c++20 -Wall -Wextra -Wpedantic -g main.cpp -o app
```

```powershell
cl /std:c++20 /W4 /permissive- /EHsc /Zi main.cpp /Fe:app.exe
```

## Program and declarations

```cpp
#include <iostream>

namespace app {
constexpr int version{1};
int add(int a, int b); // declaration
}

int main() {
    std::cout << app::add(2, 3) << '\n';
}
```

## Variables

```cpp
bool flag{true};
int count{};                    // zero
double price{3.5};
char code{'A'};
std::string name{"Ada"};
const auto size = name.size();
constexpr auto max_items = 100;
```

## Control flow

```cpp
if (condition) { } else { }
switch (value) { case 1: break; default: break; }
for (int i = 0; i < n; ++i) { }
for (const auto& item : items) { }
while (condition) { }
```

## Functions and parameters

```cpp
int square(int x) { return x * x; }              // cheap value
void show(const Large& value);                   // observe
void update(Large& value);                       // mutate
void maybe_update(Large* value);                 // optional observer
void take(std::unique_ptr<Large> value);          // take ownership
```

Return values by value. Never return a reference/view/pointer to a local.

## Class

```cpp
class Counter {
public:
    explicit Counter(int initial) : value_{initial} {}
    [[nodiscard]] int value() const noexcept { return value_; }
    void increment() noexcept { ++value_; }
private:
    int value_{};
};
```

## Common types

```cpp
std::vector<int> values{1, 2, 3};
std::array<int, 3> fixed{1, 2, 3};
std::unordered_map<std::string, int> counts;
std::optional<int> maybe;
std::variant<int, std::string> either;
std::unique_ptr<Widget> owner = std::make_unique<Widget>();
std::span<const int> view = values;
std::string_view text = "literal";
```

## Algorithms

```cpp
std::ranges::sort(values);
auto it = std::ranges::find(values, wanted);
bool any = std::ranges::any_of(values, predicate);
std::ranges::transform(input, std::back_inserter(output), transform);
std::erase_if(values, predicate);
auto total = std::accumulate(values.begin(), values.end(), 0LL);
```

## Errors

```cpp
if (!valid) throw std::invalid_argument{"reason"};
try { work(); }
catch (const SpecificError& e) { /* handle */ }
catch (const std::exception& e) { /* boundary */ }
```

Use `optional` for normal absence, `expected` (C++23) for explicit expected failures, exceptions for nonlocal exceptional failure, and assertions for programmer bugs.

## Lifetime/ownership

```text
T                 owns/contains value
T& / const T&     required non-owning access
T* / const T*     optional non-owning access (convention)
unique_ptr<T>     exclusive dynamic owner
shared_ptr<T>     shared lifetime (use only when genuine)
weak_ptr<T>       observes shared lifetime
```

## Safety defaults

- Initialize; use `const`; keep scope narrow.
- Prefer vector/string/RAII over arrays, `new`, and `delete`.
- Check input and sizes before converting/allocating/indexing.
- Do not keep iterators/views across invalidating mutation.
- Compile with warnings; test Debug and Release; run ASan+UBSan.
- Avoid `using namespace std;`, C-style casts, raw owning pointers, and clever macros.

