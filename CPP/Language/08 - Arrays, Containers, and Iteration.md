# Arrays, Containers, and Iteration

Use standard containers instead of manually allocated arrays.

The central C++ distinction is **ownership versus access**. An array or container owns elements; an iterator, pointer, span, or range view usually only describes how to reach elements. A borrowed access object must not outlive or be invalidated by its owner.

## Built-in arrays

`T values[N]` stores exactly `N` elements inline. The extent is part of the type, so `int[3]` and `int[4]` differ. Arrays cannot be copied or assigned as wholes, and most expressions convert an array to `T*` pointing at its first element. That decay loses the extent.

```cpp
template<std::size_t N>
int sum(const int (&values)[N]); // reference preserves N
```

Prefer `std::array<T, N>` when fixed-size value semantics matter. It supports copying, assignment, iterators, and container operations while retaining inline storage.

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

## `vector` capacity and relocation

`std::vector<T>` stores elements contiguously. Its size counts constructed elements; its capacity counts available storage before another allocation is needed. `reserve(n)` changes capacity without adding elements, while `resize(n)` constructs or destroys elements to change size.

Growing beyond capacity allocates a new buffer and moves or copies elements. All pointers, references, spans, and iterators into the old buffer then dangle. `push_back` can therefore invalidate a reference obtained earlier:

```cpp
auto& first = values.front();
values.push_back(42); // may reallocate
// using first may now be undefined behavior
```

Reserve when a useful upper bound is known, but do not treat capacity as part of a stable API.

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

An iterator's category describes supported operations:

- input iterators support single-pass reading;
- output iterators support single-pass writing;
- forward iterators support multipass traversal;
- bidirectional iterators also move backward;
- random-access iterators support constant-time jumps and differences;
- contiguous iterators additionally correspond to adjacent memory.

Algorithms state category requirements. `std::sort` needs random access; `std::list` therefore provides its own member `sort`.

The iterator returned by `erase` is the next valid position and is the safe way to continue:

```cpp
for (auto it = values.begin(); it != values.end();) {
    if (should_remove(*it)) {
        it = values.erase(it);
    } else {
        ++it;
    }
}
```

## Ranges, spans, and views

A range is anything providing a begin/end pair. A `std::span<T>` is a cheap borrowed view over contiguous elements and carries a size. Its static extent can be part of the type (`std::span<int, 4>`), while dynamic extent is stored at runtime.

Ranges views such as `filter` and `transform` are often lazy: they store a source range and callable, then perform work during iteration. They may borrow their source, so returning or storing a view can dangle. “View” means lightweight range adaptor behavior, not necessarily ownership.

Use `std::ranges` algorithms when projections, constrained interfaces, or direct range arguments improve clarity. Remember that many algorithms rearrange or overwrite elements but do not change container size; the erase-remove pattern or C++20 `std::erase_if` performs actual removal.

## Choosing a container

Use `vector` as the default sequence. Choose another container for a demonstrated semantic requirement:

- `array` for compile-time fixed size and value semantics;
- `deque` for efficient growth at both ends without contiguous storage;
- `list` only when stable node identity and its splice behavior outweigh allocation and locality costs;
- ordered associative containers for sorted traversal and logarithmic lookup;
- unordered containers for hash-based average constant-time lookup when ordering is irrelevant.

Container choice controls invalidation, complexity, memory layout, and reference stability—not merely available member functions.

See [[Standard Library/Containers]], [[Standard Library/Iterators and Views]], and [[Standard Library/Algorithms and Ranges]].
