# Concurrency

Concurrency allows tasks to overlap. It also creates nondeterminism: correctness depends on synchronization and lifetimes across threads.

## Fundamental rule

A **data race** occurs when threads access the same memory concurrently, at least one access writes, and the accesses are not properly synchronized. A data race is undefined behavior.

## Prefer structured threads

```cpp
#include <stop_token>
#include <thread>

std::jthread worker{[](std::stop_token stop) {
    while (!stop.stop_requested()) {
        do_one_unit();
    }
}}; // requests stop and joins at scope exit
```

`std::jthread` (C++20) is safer than `std::thread` because its destructor joins and it supports cooperative cancellation.

## Shared state

```cpp
std::mutex mutex;
std::vector<int> results;

void add_result(int value) {
    std::lock_guard lock{mutex};
    results.push_back(value);
}
```

Protect every access participating in an invariant with the same mutex. Keep critical sections short. Use `std::scoped_lock` to acquire multiple mutexes without common deadlocks. Never manually pair `lock()`/`unlock()` when an RAII lock works.

## Waiting

Condition variables wait for state changes. Always use a predicate because wakeups may be spurious:

```cpp
std::unique_lock lock{mutex};
cv.wait(lock, [&] { return ready; });
```

## Atomics

`std::atomic<T>` provides indivisible operations and memory ordering. It does not automatically protect multi-variable invariants. Default sequential consistency is easiest to reason about. Weaker memory ordering is expert-level and should follow measurement plus rigorous proof.

Prefer message passing, immutable data, task systems, and partitioned ownership over pervasive shared mutable state. See [[Standard Library/Concurrency Library]].

