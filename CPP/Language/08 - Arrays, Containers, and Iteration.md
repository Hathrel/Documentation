# Arrays, Containers, and Iteration

Use standard containers instead of manually allocated arrays.

```cpp
#include <array>
#include <span>
#include <vector>

std::array<int, 3> fixed{1, 2, 3};
std::vector<int> dynamic{1, 2, 3};
```

- `std::array<T, N>`: fixed size known at compile time; owns elements inline.
- `std::vector<T>`: variable-size contiguous sequence; best default sequence.
- `std::span<T>`: non-owning view over contiguous elements; ideal function parameter.

```cpp
int sum(std::span<const int> values) {
    int total{};
    for (const int value : values) total += value;
    return total;
}
```

The same function accepts arrays, vectors, and compatible contiguous storage without copying.

## Indexing

`container[i]` does not check bounds. `container.at(i)` throws `std::out_of_range`. Range-based loops and algorithms reduce indexing mistakes.

```cpp
for (const auto& value : values) { /* read */ }
for (auto& value : values) { /* modify */ }
for (auto value : values) { /* copy */ }
```

## Iterators

Iterators generalize pointers into sequences. A half-open range `[begin, end)` includes `begin` but excludes `end`; an empty range has `begin == end`.

```cpp
const auto it = std::find(values.begin(), values.end(), wanted);
if (it != values.end()) std::cout << *it;
```

Mutations can invalidate iterators, pointers, and references. For example, vector reallocation invalidates all of them; erasing invalidates the erased position and everything after it. Check the container operation’s invalidation rules.

See [[Standard Library/Containers]], [[Standard Library/Iterators and Views]], and [[Standard Library/Algorithms and Ranges]].

