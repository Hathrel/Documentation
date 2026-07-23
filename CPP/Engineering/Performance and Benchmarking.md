# Performance and Benchmarking

Correctness and clarity come first. Optimization begins with a measured workload and ends with a measured improvement.

## Process

1. Define latency, throughput, memory, power, or size target.
2. Measure a representative release build.
3. Profile to locate hotspots.
4. Change one important factor.
5. Verify correctness and benchmark statistically.

## Usually important

- Algorithmic complexity and avoiding repeated work.
- Data layout and cache locality; contiguous `vector` often wins.
- Allocation frequency and ownership churn.
- I/O batching and serialization.
- Contention, false sharing, and task granularity.
- Avoiding accidental copies of large objects.

Micro-optimizing syntax outside a hotspot rarely matters.

## Benchmarking

Use `std::chrono::steady_clock`, warm up if relevant, repeat, and consume results so optimization cannot remove the work. A benchmark framework such as Google Benchmark handles many pitfalls. Control CPU load, frequency scaling, input distribution, and compiler flags; report variability, not one number.

```cpp
const auto start = std::chrono::steady_clock::now();
auto result = work(input);
do_not_optimize_away(result); // framework facility preferred
const auto elapsed = std::chrono::steady_clock::now() - start;
```

## Complexity reminders

Big-O predicts scaling, not constant factors. An O(n log n) algorithm can beat O(n) for small n and vice versa. Hash tables have average-case claims and adversarial considerations. Parallelism adds synchronization and scheduling overhead.

Never trade away defined behavior. “Faster” UB has no valid performance contract.

