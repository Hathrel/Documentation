# Random Numbers

The random library separates **engines** (uniform pseudo-random bits) from **distributions** (domain-shaped values).

```cpp
#include <random>

std::random_device seed_source;
std::mt19937 engine{seed_source()};
std::uniform_int_distribution<int> die{1, 6};

const int roll = die(engine);
```

Create and seed an engine once, then reuse it. Reseeding for every value can reduce quality and performance.

## Common distributions

- `uniform_int_distribution`: inclusive integer interval.
- `uniform_real_distribution`: real interval (endpoint details matter).
- `bernoulli_distribution`: true with probability p.
- `normal_distribution`, `exponential_distribution`, and others for statistical models.
- `std::shuffle`: uniformly shuffle a range using an engine.

For repeatable tests/simulations, use and record a fixed seed:

```cpp
std::mt19937 engine{0xC0FFEEu};
```

Do not use `std::rand`; it has weak interfaces and quality. Do not use general-purpose standard engines for passwords, tokens, keys, or other security needs. Use an operating-system cryptographic random generator or vetted cryptographic library. `std::random_device` is not universally guaranteed to be cryptographically secure or nondeterministic.

In concurrent code, give each thread its own engine or synchronize access. Copying an engine copies its state and future sequence.

