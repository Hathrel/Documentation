# Containers

Containers own elements and provide iteration. Their tradeoffs concern access pattern, memory layout, lookup, insertion, ordering, and iterator invalidation.

## Sequence containers

| Container | Use when | Key facts |
|---|---|---|
| `std::vector<T>` | default dynamic sequence | contiguous, fast indexing/end insertion |
| `std::array<T,N>` | fixed compile-time length | contiguous, value semantics |
| `std::deque<T>` | frequent insertion at both ends | indexed, segmented storage |
| `std::list<T>` | stable iterators and splicing are essential | node allocation, no indexing; rarely fastest |
| `std::forward_list<T>` | minimal singly linked list is required | forward-only; specialized use |

Container adaptors: `std::stack`, `std::queue`, and `std::priority_queue` restrict an underlying sequence to a purpose-built interface.

## Associative containers

| Container | Lookup | Order |
|---|---|---|
| `std::map<K,V>` / `std::set<K>` | O(log n) | sorted by comparator |
| `std::unordered_map<K,V>` / `std::unordered_set<K>` | average O(1), worst O(n) | unspecified; uses hash/equality |
| `std::multimap` / `multiset` variants | O(log n) | duplicate keys allowed |

```cpp
std::unordered_map<std::string, int> counts;
++counts["apple"]; // [] inserts a default value when missing

if (const auto it = counts.find("pear"); it != counts.end()) {
    std::cout << it->second;
}
```

Use `.at(key)` for checked lookup without insertion; `contains(key)` (C++20) for membership. `try_emplace` constructs a value only when the key is absent. Do not depend on unordered iteration order.

## Vector essentials

```cpp
std::vector<Item> items;
items.reserve(100);            // capacity, not size
items.emplace_back(args...);
items.push_back(item);
items.erase(std::remove(items.begin(), items.end(), unwanted), items.end());
// C++20: std::erase(items, unwanted);
```

`reserve` can reduce reallocations; do not resize when you merely want capacity. Prefer `push_back(existing_value)` and use `emplace_back` when constructing directly from arguments.

## Complexity and invalidation

Big-O does not capture cache locality and allocation costs; vector often beats linked structures. Each modifying operation has exact iterator/reference invalidation rules—consult a reference before retaining handles across mutations.

Related: [[Language/08 - Arrays, Containers, and Iteration]], [[Standard Library/Algorithms and Ranges]].

