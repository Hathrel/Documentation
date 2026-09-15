# Standard Library Map

The standard library supplies containers, algorithms, utilities, I/O, concurrency, and more. `std::` names live in the standard namespace. Include the documented header for every facility you use.

## How to read a library contract

An API reference entry is more than a signature. Check:

- **requirements and preconditions:** which arguments, iterator categories, type operations, and object states are valid;
- **ownership and lifetime:** whether the result owns, borrows, or refers into an argument;
- **invalidation:** which mutations invalidate iterators, pointers, references, spans, and views;
- **complexity:** whether the guarantee is constant, logarithmic, linear, amortized, or average-case;
- **exceptions:** what may throw and which state guarantee remains after failure;
- **thread safety:** which operations can occur concurrently on the same or different objects;
- **standard version:** when a facility or overload became available.

Violating a documented precondition can be undefined behavior even when the function appears to work. Complexity and invalidation guarantees are part of the portable API, while implementation details such as small-string buffer size are not.

## Core reference

- [[Standard Library/Containers]]
- [[Standard Library/Algorithms and Ranges]]
- [[Standard Library/Iterators and Views]]
- [[Standard Library/Strings and Character Data]]
- [[Standard Library/Input and Output]]
- [[Standard Library/Files and Filesystem]]
- [[Standard Library/Memory and Smart Pointers]]
- [[Standard Library/Utility Types]]
- [[Standard Library/Numeric and Math]]
- [[Standard Library/Time and Dates]]
- [[Standard Library/Text, Formatting, and Regular Expressions]]
- [[Standard Library/Random Numbers]]
- [[Standard Library/Concurrency Library]]
- [[Standard Library/Type Support and Low-Level Utilities]]
- [[Standard Library/Errors and Diagnostics]]

## Selection principles

- Default sequence: `std::vector`.
- Default owning text: `std::string`; read-only parameter: often `std::string_view`.
- Default ownership: direct value, then `std::unique_ptr` if indirection is needed.
- Default transformation/search: a ranges algorithm.
- Default clock for durations/timeouts: `std::chrono::steady_clock`.
- Default random engine: usually `std::mt19937` seeded once; distribution chosen for the domain.

For an at-a-glance list, see [[91 - Standard Library Quick Reference]].
