# Concurrency

Concurrency allows tasks to overlap. It also creates nondeterminism: correctness depends on synchronization and lifetimes across threads.

## Threads and the memory model

Each thread has its own execution, but all threads participate in one C++ abstract machine. Compiler and CPU reordering are allowed when single-thread observable behavior is preserved. Synchronization creates **happens-before** relationships that make writes visible in a defined way.

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

`std::thread` must be joined or detached before its destructor; destroying a joinable thread calls `std::terminate`. Detaching discards structured lifetime control and is rarely appropriate. `std::jthread` requests stop and joins, but code must still ensure every referenced object outlives the worker.

Passing `x` to a thread copies or moves it into internal storage. Use `std::ref(x)` only for an intentional borrow with a proven lifetime.

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

A mutex protects a protocol, not merely a variable. Every access to state participating in an invariant must follow the same locking rule. Returning a reference to protected data after releasing the lock defeats the protection.

Deadlock requires a cycle of waiting. Establish a global lock order or acquire multiple mutexes together with `std::scoped_lock`. Avoid calling unknown code while holding a lock.

## Waiting

Condition variables wait for state changes. Always use a predicate because wakeups may be spurious:

```cpp
std::unique_lock lock{mutex};
cv.wait(lock, [&] { return ready; });
```

The predicate and the notification share a state protected by the mutex. The waiting thread checks the predicate while locked, atomically releases the mutex while sleeping, then reacquires it before returning. Notifications are hints; the predicate is the truth.

## Atomics

`std::atomic<T>` provides indivisible operations and memory ordering. It does not automatically protect multi-variable invariants. Default sequential consistency is easiest to reason about. Weaker memory ordering is expert-level and should follow measurement plus rigorous proof.

Prefer message passing, immutable data, task systems, and partitioned ownership over pervasive shared mutable state. See [[Standard Library/Concurrency Library]].

Atomic operations prevent data races on that atomic object and can establish ordering for other memory. They do not combine several objects into one invariant. The default sequentially consistent order provides a single global order of such atomic operations and is the right baseline.

Acquire/release and relaxed ordering require a precise proof of which operation synchronizes with which and what data becomes visible. “Atomic means thread-safe” is not a sufficient design argument. Use mutexes unless a measured need and a reviewed memory-order protocol justify atomics.
