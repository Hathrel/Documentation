# Standard Library Quick Reference

Include the header for each facility you use.

| Need | Facility | Header |
|---|---|---|
| Dynamic contiguous sequence | `std::vector` | `<vector>` |
| Fixed sequence | `std::array` | `<array>` |
| Double-ended sequence | `std::deque` | `<deque>` |
| Sorted key/value | `std::map` | `<map>` |
| Hash key/value | `std::unordered_map` | `<unordered_map>` |
| Unique sorted/hash keys | `std::set`, `unordered_set` | `<set>`, `<unordered_set>` |
| Owning text | `std::string` | `<string>` |
| Non-owning text | `std::string_view` | `<string_view>` |
| Non-owning contiguous range | `std::span` | `<span>` |
| Maybe a value | `std::optional` | `<optional>` |
| One of known types | `std::variant` | `<variant>` |
| Success or typed error (C++23) | `std::expected` | `<expected>` |
| Exclusive ownership | `std::unique_ptr` | `<memory>` |
| Shared ownership | `std::shared_ptr`, `weak_ptr` | `<memory>` |
| Sort/search/transform | algorithms/ranges | `<algorithm>`, `<ranges>` |
| Fold and numeric algorithms | `accumulate`, scans, etc. | `<numeric>` |
| Console/stream I/O | `cin`, `cout`, streams | `<iostream>`, `<sstream>` |
| File I/O | `ifstream`, `ofstream` | `<fstream>` |
| Paths/directories | `std::filesystem` | `<filesystem>` |
| Type-safe format (C++20) | `std::format` | `<format>` |
| Number conversion | `from_chars`, `to_chars` | `<charconv>` |
| Time/durations | `std::chrono` | `<chrono>` |
| Randomness | engines/distributions | `<random>` |
| Math/constants | math, `std::numbers` | `<cmath>`, `<numbers>` |
| Threads | `thread`, `jthread` | `<thread>` |
| Locks | mutex/RAII locks | `<mutex>`, `<shared_mutex>` |
| Coordination | condition variable | `<condition_variable>` |
| Atomics | `std::atomic` | `<atomic>` |
| Futures/tasks | `future`, `async` | `<future>` |
| Compile-time type queries | type traits/concepts | `<type_traits>`, `<concepts>` |
| Source file/line | `source_location` | `<source_location>` |
| Bit operations | bit utilities | `<bit>` |
| Fixed-width integers | `int32_t`, etc. | `<cstdint>` |
| Numeric limits | `numeric_limits` | `<limits>` |
| Error codes | `error_code`, `errc` | `<system_error>` |

## Choose a container

```text
Need a sequence?                 vector
Fixed size known at compile time? array
Frequent insertion both ends?    deque
Sorted keys/range queries?        map/set
Fast average exact-key lookup?    unordered_map/unordered_set
Priority access?                  priority_queue
Stable-node/splice requirement?   list (verify by measurement/design)
```

## Complexity snapshot

| Operation | vector | map | unordered_map |
|---|---:|---:|---:|
| indexed access | O(1) | — | — |
| key lookup | O(n) | O(log n) | average O(1), worst O(n) |
| append | amortized O(1) | — | — |
| middle insert/erase | O(n) | O(log n) | average O(1), worst O(n) |

Exact rules, invalidation, overloads, and preconditions matter; use [[Standard Library/Standard Library Map]] and a current formal reference.

