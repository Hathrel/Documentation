# Concurrency Library

Start with [[Language/19 - Concurrency]] for correctness concepts.

## Facilities

- Threads: `std::thread`, `std::jthread`, `std::this_thread`.
- Mutual exclusion: `mutex`, `shared_mutex`, timed variants.
- RAII locks: `lock_guard`, `unique_lock`, `scoped_lock`, `shared_lock`.
- Coordination: `condition_variable`, semaphores, latches, barriers.
- Futures: `promise`, `future`, `shared_future`, `packaged_task`, `async`.
- Atomic operations: `std::atomic`, atomic flags, fences, wait/notify.
- Cancellation: `stop_source`, `stop_token`, `stop_callback`.

## Async caveat

```cpp
auto future = std::async(std::launch::async, compute, input);
auto result = future.get(); // also propagates exceptions
```

Specify a launch policy when execution behavior matters. `std::async` is useful for a few coarse tasks but is not a general thread pool.

## Condition-variable queue pattern

The shared queue, stop flag, and predicate are protected by one mutex. Producers modify state under lock then notify; consumers wait with a predicate, move work out under lock, release the lock, then process it. Define shutdown behavior explicitly.

## Deadlock prevention

- Prefer one lock at a time.
- Establish a global lock order.
- Use `std::scoped_lock{a, b}` for multiple locks.
- Never call unknown/user code while holding a lock.
- Keep locks out of slow I/O and callbacks.

## Parallel algorithms

Execution policies (`std::execution::par`, etc.) permit parallel or unsequenced execution for some algorithms. Callbacks must meet strict independence and safety requirements; implementation availability varies. Benchmark before adopting them.

Concurrency tests cannot prove absence of races. Combine design review, stress tests, and ThreadSanitizer where supported.

