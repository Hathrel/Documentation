# Undefined Behavior and the Object Model

**Undefined behavior (UB)** means the C++ standard imposes no requirements on the program. It may appear to work, crash, leak secrets, or change behavior under optimization.

Common sources:

- out-of-bounds access;
- use after free or after lifetime end;
- dereferencing null or invalid pointers;
- signed integer overflow and invalid shifts;
- uninitialized reads;
- data races;
- violating type/lifetime/aliasing rules;
- falling off a value-returning function (except `main`);

```cpp
std::vector<int> values{1, 2, 3};
// int bad = values[3]; // UB
int checked = values.at(2);
```

## Do not reason from one run

The optimizer assumes UB never happens. Code after a violated precondition may be transformed in surprising ways. A successful test run proves little about UB.

## Object representation

Objects occupy storage with size, alignment, lifetime, and a type. `sizeof(T)` includes padding. Do not serialize a struct by writing its raw bytes: padding, byte order, sizes, and layouts vary. Serialize fields explicitly.

Use `std::byte` for raw byte storage, `std::bit_cast` for same-size trivially copyable representation conversion, and `std::memcpy` when its object-model rules apply. Do not use `reinterpret_cast` as a general type-punning device.

## Aliasing and alignment

Access memory through a type the object model permits. Misaligned pointers and invalid aliasing are UB even if the hardware tolerates them. Use standard abstractions or carefully reviewed platform APIs for low-level memory.

## Find problems

Enable warnings and run AddressSanitizer and UndefinedBehaviorSanitizer during development. Use ThreadSanitizer where supported for concurrency. See [[Engineering/Warnings, Sanitizers, and Static Analysis]].

