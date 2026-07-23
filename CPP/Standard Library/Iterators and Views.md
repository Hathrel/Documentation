# Iterators and Views

An iterator denotes a position. Algorithms operate on iterator/sentinel pairs or ranges.

## Categories

- Input/output: single-pass read or write.
- Forward: multi-pass, moves forward.
- Bidirectional: moves forward/backward.
- Random access: constant-time jumps/index differences.
- Contiguous: random access plus contiguous storage.

An algorithm’s requirements determine which containers it accepts. `std::vector` iterators are contiguous; `std::list` iterators are bidirectional.

## Operations

```cpp
auto first = std::begin(values);
auto last = std::end(values);
if (first != last) {
    std::cout << *first;
    ++first;
}

auto next = std::next(first, 3);
auto distance = std::distance(first, last);
```

Never dereference the end iterator. Increment only while a valid next position exists.

## Range concepts and borrowed lifetimes

C++20 ranges may use a different sentinel type for the end. `std::ranges::begin`, `end`, `size`, `empty`, and `data` provide uniform customization-aware access.

Views are lightweight, composable, usually non-owning range adaptors such as `filter`, `transform`, `take`, `drop`, `reverse`, `split`, and `iota`. Because laziness retains source relationships, mutations to the source can affect later results and can invalidate the view’s iteration.

```cpp
auto first_ten_even = std::views::iota(0)
                    | std::views::filter([](int n) { return n % 2 == 0; })
                    | std::views::take(10);
```

Avoid returning a view that refers to a destroyed local range. `std::span` and `std::string_view` have the same fundamental lifetime concern.

