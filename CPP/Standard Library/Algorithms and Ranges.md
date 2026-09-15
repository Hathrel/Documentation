# Algorithms and Ranges

Algorithms express *what* operation is performed while containers own data. Include `<algorithm>`, `<numeric>`, and for C++20 ranges, `<ranges>`.

```cpp
std::vector<int> values{4, 1, 3, 2};
std::ranges::sort(values);

const auto it = std::ranges::find(values, 3);
const bool any_negative = std::ranges::any_of(values, [](int x) { return x < 0; });
const auto total = std::accumulate(values.begin(), values.end(), 0);
```

Classic algorithms take iterator pairs; ranges algorithms can accept a range directly and constrain invalid calls earlier. Algorithms do not own inputs. Returned iterators and subranges usually borrow from the supplied range and inherit its lifetime.

## Callable and ordering contracts

A sorting comparator must define a strict weak ordering; using `<=` instead of `<` can make behavior undefined. Projections compare a selected member without building an intermediate range. Algorithms may copy callable objects, so do not depend on observing mutations to one particular callable instance.

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

Views are usually lazy and non-owning. Work occurs during iteration, can repeat on another traversal, and may produce proxies rather than references. Materialize an owning result when it must outlive the source.

“Remove” algorithms rearrange retained values and return a logical end; they do not change container size. Follow with erasure or use C++20 `std::erase`/`std::erase_if`.

## Common traps

- Sorting requires random-access iterators; use a container’s member function where applicable (`list.sort()`).
- Binary search algorithms require a correctly sorted range with the matching comparator.
- `remove` shifts unwanted values to the end; it cannot resize a container. Use `std::erase`/`std::erase_if` in C++20.
- Choose the accumulator type deliberately: initial `0` makes `accumulate` use `int`; use `0.0` or `std::int64_t{0}` when appropriate.
