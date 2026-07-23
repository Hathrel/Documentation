# Project Ideas and Exercises

Build each project in stages. Keep warnings clean, add tests for core logic, and run sanitizers.

## Exercises by topic

### Fundamentals

1. Temperature converter with validated input and named unit enum.
2. Number guessing game with a seeded random engine for repeatable tests.
3. Calculator supporting `+ - * /`, clear error handling, and separate parse/evaluate functions.
4. Print a multiplication table without duplicated formatting logic.

### Containers and algorithms

1. Word frequency: normalize tokens, count them, sort by frequency then name.
2. Contact book with add/find/remove/list and file persistence.
3. Grade report computing median, mean, min/max, and per-letter counts.
4. Log analyzer grouping messages by severity and hour.

### Types and design

1. `Money` type using integer cents with comparison, addition, formatting, and overflow policy.
2. Validated `EmailAddress` value type.
3. Shape collection using first `variant`, then virtual polymorphism; compare designs.
4. LRU cache with explicit capacity, ownership, and invalidation tests.

### Concurrency

1. Parallel file hasher with a bounded work queue.
2. Producer/consumer pipeline supporting clean cancellation.
3. Thread-safe metrics accumulator; use TSan and document its invariant.

## Capstone 1 — Task tracker CLI

Requirements:

- Commands: add, list, complete, remove, help.
- `Task` has an ID, description, status, and timestamp.
- Parse arguments separately from execution.
- Persist through a versioned, validated format.
- Write changes safely through a temporary file.
- Sort/filter with standard algorithms.
- Unit-test parsing and domain logic; integration-test persistence.

## Capstone 2 — Text indexer

- Recursively scan a chosen directory with size/extension limits.
- Tokenize files, count terms per document, and build an inverted index.
- Query AND/OR terms and rank results.
- Keep the initial version single-threaded, profile it, then parallelize only if useful.
- Handle unreadable/mutating files and cancellation.

## Definition of done

- Clean build from an empty build directory.
- No unexplained warnings.
- Helpful usage and failure messages.
- No sanitizer findings under representative tests.
- README with contract, build/run commands, examples, and limitations.
- Tests cover success, boundaries, malformed input, and regressions.

See the runnable [[Examples/word-frequency/README|word-frequency example]].

