# Coroutines

Coroutines are functions that can suspend and resume while preserving local state. C++20 provides language machinery, not a general scheduler or standard networking runtime.

A coroutine is recognized by `co_await`, `co_yield`, or `co_return`. Its behavior is determined by a **promise type** associated with its return type.

```cpp
Task<int> fetch_count() {
    auto response = co_await fetch("/count");
    co_return parse_count(response);
}
```

This is conceptual: `Task` and the awaitable `fetch` must come from a library or your own carefully implemented types.

## Terms

- **coroutine frame:** stored parameters, locals, and state across suspension; may be dynamically allocated.
- **promise object:** controls produced results, exceptions, initial and final suspension, and frame behavior.
- **awaitable/awaiter:** determines readiness, suspension, and resume result for `co_await`.
- `co_yield value`: commonly publishes a value and suspends through `yield_value`.

## Lifetime rules

Suspension makes lifetimes especially important. References and pointers stored in the frame must remain valid until their last use. A coroutine return object usually owns or controls its frame; destroying it must follow the type’s contract. Cancellation and destruction while I/O is pending need explicit design.

## Exceptions and threads

Uncaught exceptions are delivered to the promise’s `unhandled_exception`; the coroutine abstraction decides how callers observe them. Resumption may occur on another thread depending on the awaitable or runtime. Coroutines provide concurrency structure, not automatic synchronization.

## When to use

Coroutines can make asynchronous workflows, generators, and state machines read sequentially. Use a mature runtime whose scheduling, cancellation, ownership, and error contracts you understand. Writing a correct general-purpose coroutine task type is advanced library work.

For ordinary parallel computation or a simple background worker, [[Language/19 - Concurrency|structured threads and tasks]] may be simpler.

