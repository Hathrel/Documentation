# Coroutines

Coroutines are functions that can suspend and resume while preserving local state. C++20 provides language machinery, not a general scheduler or standard networking runtime.

A coroutine is recognized by `co_await`, `co_yield`, or `co_return`. Its behavior is determined by a **promise type** associated with its return type.

Calling a coroutine does not execute it like an ordinary function call. It creates or obtains a coroutine frame, initializes parameters and the promise, and follows the promise's `initial_suspend` policy. The returned object usually acts as an owner or handle to that frame.

```cpp
Task<int> fetch_count() {
    auto response = co_await fetch("/count");
    co_return parse_count(response);
}
```

This is conceptual: `Task` and the awaitable `fetch` must come from a library or your own carefully implemented types.

## The await protocol

For `co_await expression`, the language obtains an awaiter and conceptually calls:

1. `await_ready()` to decide whether suspension can be skipped;
2. `await_suspend(handle)` after saving coroutine state;
3. `await_resume()` to produce the expression's result when execution continues.

The exact transformation includes customization through the promise and operator lookup. `await_suspend` can arrange continuation scheduling and may resume another coroutine. An awaiter must obey strict lifetime and reentrancy rules; resumption can occur before `await_suspend` returns in some designs.

## Terms

- **coroutine frame:** stored parameters, locals, and state across suspension; may be dynamically allocated.
- **promise object:** controls produced results, exceptions, initial and final suspension, and frame behavior.
- **awaitable/awaiter:** determines readiness, suspension, and resume result for `co_await`.
- `co_yield value`: commonly publishes a value and suspends through `yield_value`.

## Lifetime rules

Suspension makes lifetimes especially important. References and pointers stored in the frame must remain valid until their last use. A coroutine return object usually owns or controls its frame; destroying it must follow the type’s contract. Cancellation and destruction while I/O is pending need explicit design.

Parameters copied by value normally live in the frame; reference parameters remain references. A coroutine lambda can be especially dangerous: captures belong to the closure object, not automatically to the coroutine frame, and the closure may die after the initial call. Prefer passing needed state as value parameters to a named coroutine or otherwise prove the closure lifetime.

The frame is destroyed only according to the abstraction's ownership policy. Destroying a suspended frame runs destructors for live frame objects. Resuming a completed coroutine or destroying a frame twice is invalid.

## Promise responsibilities

The promise type defines `get_return_object`, initial and final suspension, result handling, and `unhandled_exception`. A generator's `yield_value` commonly stores a current element and returns an awaiter that suspends.

`final_suspend` is critical: it coordinates continuation and determines when destroying the frame is safe. A production task type must also define single versus multiple awaiting, eager versus lazy start, cancellation, result ownership, and behavior when the return object is abandoned.

## Exceptions and threads

Uncaught exceptions are delivered to the promise’s `unhandled_exception`; the coroutine abstraction decides how callers observe them. Resumption may occur on another thread depending on the awaitable or runtime. Coroutines provide concurrency structure, not automatic synchronization.

## When to use

Coroutines can make asynchronous workflows, generators, and state machines read sequentially. Use a mature runtime whose scheduling, cancellation, ownership, and error contracts you understand. Writing a correct general-purpose coroutine task type is advanced library work.

For ordinary parallel computation or a simple background worker, [[Language/19 - Concurrency|structured threads and tasks]] may be simpler.
