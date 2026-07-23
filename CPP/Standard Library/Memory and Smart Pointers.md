# Memory and Smart Pointers

Prefer automatic storage and value members. Dynamic allocation is justified by dynamic lifetime, polymorphism, large/recursive structures, or stable indirection—not merely because an object is large.

## `unique_ptr`

```cpp
auto item = std::make_unique<Item>(args);
process(*item);
auto transferred = std::move(item); // item is now null
```

`std::unique_ptr<T>` expresses one owner, is movable but not copyable, and has near-raw-pointer overhead. `unique_ptr<T[]>` manages a dynamic array, but `std::vector<T>` is usually better. Custom deleters adapt C handles and specialized resources.

## `shared_ptr` and `weak_ptr`

```cpp
auto shared = std::make_shared<Node>();
std::weak_ptr<Node> observer = shared;
if (auto locked = observer.lock()) locked->visit();
```

`shared_ptr` uses reference-counted shared ownership. It adds allocation/atomic bookkeeping and can leak cycles. Break ownership cycles with `weak_ptr`. Do not create two independent `shared_ptr`s from the same raw pointer. Use `enable_shared_from_this` only when an object already managed by a `shared_ptr` must issue another shared owner.

## Allocators and polymorphic memory resources

Standard containers accept allocators. `<memory_resource>` provides `std::pmr` types and resources such as monotonic and pool resources for allocation-policy control. Use after profiling; resource lifetimes must exceed all containers using them.

## Object construction utilities

`std::construct_at`, `destroy_at`, `uninitialized_copy`, and related functions support low-level storage management. Most application code should not use them directly.

## Ownership in APIs

- Accept `T&`/`const T&`/`T*` for non-owning access.
- Accept `unique_ptr<T>` by value to take exclusive ownership.
- Accept `shared_ptr<T>` by value only when retaining shared ownership.
- Return `unique_ptr<T>` for dynamically allocated exclusive ownership.

See [[Language/06 - References, Pointers, and Lifetimes]] and [[Language/11 - Resource Management and RAII]].

