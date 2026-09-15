# Containers

Containers own elements and provide iteration. Their tradeoffs concern access pattern, memory layout, lookup, insertion, ordering, and iterator invalidation.

## Container contracts

Containers own elements and expose iterators, references, and pointers that borrow from that storage. Every mutating operation has an invalidation contract; never infer stability from a test run.

Most containers are allocator-aware and express requirements on element construction, movement, comparison, or hashing. An operation may be unavailable or offer a weaker exception guarantee when the element type does not meet those requirements.

`size()` counts elements and normally uses an unsigned `size_type`. `empty()` states intent better than comparing size with zero. Iterators form half-open ranges, and `end()` is a sentinel position that must not be dereferenced.

## Sequence containers

| Container | Use when | Key facts |
|---|---|---|
| `std::vector<T>` | default dynamic sequence | contiguous, fast indexing/end insertion |
| `std::array<T,N>` | fixed compile-time length | contiguous, value semantics |
| `std::deque<T>` | frequent insertion at both ends | indexed, segmented storage |
| `std::list<T>` | stable iterators and splicing are essential | node allocation, no indexing; rarely fastest |
| `std::forward_list<T>` | minimal singly linked list is required | forward-only; specialized use |

Container adaptors: `std::stack`, `std::queue`, and `std::priority_queue` restrict an underlying sequence to a purpose-built interface.

`vector` and `array` are contiguous. `deque` is random-access but not contiguous. `list` and `forward_list` allocate nodes and provide stable references across many insertions, but trade away locality and random access. Choose from required semantics and measured behavior, not folklore about insertion complexity.

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

Ordered containers use a strict weak ordering. If the comparator says neither key is less than the other, the keys are equivalent for the container even if `operator==` disagrees.

Unordered containers require equal keys to have equal hash values. Rehashing invalidates iterators but not references and pointers to elements. Poor or adversarial hashing can turn average constant-time operations into linear work.

`operator[]` inserts a default-constructed mapped value on a miss; use `at` or `find` for lookup without insertion.

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

`emplace_back(args...)` constructs from arguments at the destination, while `push_back(value)` copies or moves a completed value. Emplacement is not automatically faster and can hide conversions; use the clearer operation.

## Complexity and invalidation

Big-O does not capture cache locality and allocation costs; vector often beats linked structures. Each modifying operation has exact iterator/reference invalidation rules—consult a reference before retaining handles across mutations.

Erasure invalidation differs sharply: vector invalidates the erased element and everything after it; deque rules depend on position; list invalidates only erased elements; associative containers preserve other element references and iterators. Swapping containers and allocator propagation add further rules—consult the specific operation when retaining borrows.

Related: [[Language/08 - Arrays, Containers, and Iteration]], [[Standard Library/Algorithms and Ranges]].
