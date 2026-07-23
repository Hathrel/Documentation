# Testing

Tests provide fast evidence that behavior and contracts still hold.

## Layers

- **Unit:** one small component; fast and deterministic.
- **Integration:** components plus files, database, network, or OS boundaries.
- **End-to-end:** user-visible workflow; few and valuable.
- **Property/fuzz:** generated inputs check invariants and parser robustness.
- **Regression:** reproduces a fixed bug.

## Arrange, act, assert

```cpp
void test_deposit_increases_balance() {
    BankAccount account{1'000};
    account.deposit(250);
    assert(account.balance_cents() == 1'250);
}
```

For a real suite, use a test framework such as Catch2, GoogleTest, or doctest for discovery and diagnostics. Keep core logic framework-independent.

## What to test

- representative success paths;
- boundaries: empty, one, maximum/minimum, just inside/outside;
- invalid and malformed input;
- errors and cleanup;
- invariants/properties rather than implementation details;
- move/copy/lifetime behavior for resource types;
- concurrency shutdown and cancellation.

## Testability design

Separate pure logic from I/O. Pass clocks, random engines, and external services as dependencies so tests control them. Use temporary directories and unique test data. Do not add sleeps to “fix” timing tests; coordinate explicit events or use bounded waits.

Code coverage finds unexecuted code, not missing assertions or specifications. Mutation testing and fuzzing can reveal weak tests.

Run tests under sanitizers and in Debug plus Release-like configurations. A test should be independent, deterministic, clear, and fast enough for its layer.

