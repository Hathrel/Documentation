# Algorithms and Ranges

Algorithms express *what* operation is performed while containers own data. Include `<algorithm>`, `<numeric>`, and for C++20 ranges, `<ranges>`.

```cpp
std::vector<int> values{4, 1, 3, 2};
std::ranges::sort(values);

const auto it = std::ranges::find(values, 3);
const bool any_negative = std::ranges::any_of(values, [](int x) { return x < 0; });
const auto total = std::accumulate(values.begin(), values.end(), 0);
```

## Everyday families

- Search/query: `find`, `find_if`, `count`, `all_of`, `any_of`, `none_of`, `equal`.
- Reorder: `sort`, `stable_sort`, `partial_sort`, `nth_element`, `reverse`, `rotate`, `shuffle`.
- Copy/transform: `copy`, `move`, `transform`, `fill`, `generate`.
- Remove: `remove`, `remove_if`, `unique` (then erase for containers).
- Sets on sorted ranges: `set_union`, `set_intersection`, `includes`.
- Heaps: `make_heap`, `push_heap`, `pop_heap`, or use `priority_queue`.
- Numeric: `accumulate`, `reduce`, `inner_product`, `iota`, scans.

Ranges algorithms accept a range directly and often support projections:

```cpp
std::ranges::sort(employees, std::ranges::less{}, &Employee::last_name);
```

## Views (lazy pipelines)

```cpp
auto result = values
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; });

for (int x : result) std::cout << x << ' ';
```

Views usually do not own elements and are evaluated as iterated. Ensure their underlying ranges and captured state live long enough. Materialize into a container when ownership or repeated stable results are needed.

## Common traps

- Sorting requires random-access iterators; use a container’s member function where applicable (`list.sort()`).
- Binary search algorithms require a correctly sorted range with the matching comparator.
- `remove` shifts unwanted values to the end; it cannot resize a container. Use `std::erase`/`std::erase_if` in C++20.
- Choose the accumulator type deliberately: initial `0` makes `accumulate` use `int`; use `0.0` or `std::int64_t{0}` when appropriate.

