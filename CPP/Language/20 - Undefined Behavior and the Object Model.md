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

## Behavior categories

**Ill-formed** programs violate a diagnosable rule and must be rejected, unless the rule explicitly says no diagnostic is required. **Implementation-defined** behavior requires the implementation to choose and document an outcome. **Unspecified** behavior permits one of several valid outcomes without documentation. **Undefined behavior** imposes no requirements.

Do not use “undefined” to mean merely unknown or platform-dependent. The category determines whether any portable reasoning remains possible.

## Do not reason from one run

The optimizer assumes UB never happens. Code after a violated precondition may be transformed in surprising ways. A successful test run proves little about UB.

## Lifetime and storage

Storage is raw capacity; an object exists only during its lifetime. Construction begins lifetime and destruction ends it. Reusing storage through placement construction, a union, or an allocator can leave old pointers referring to storage but not automatically to the new object.

Most code should let containers and allocators manage these transitions. Low-level code may need facilities such as `std::construct_at`, `std::destroy_at`, and in narrow replacement cases `std::launder`.

## Object representation

Objects occupy storage with size, alignment, lifetime, and a type. `sizeof(T)` includes padding. Do not serialize a struct by writing its raw bytes: padding, byte order, sizes, and layouts vary. Serialize fields explicitly.

Use `std::byte` for raw byte storage, `std::bit_cast` for same-size trivially copyable representation conversion, and `std::memcpy` when its object-model rules apply. Do not use `reinterpret_cast` as a general type-punning device.

Object representation is the sequence of unsigned-character or byte-sized units underlying an object; value representation is the bits that participate in representing its value. Padding can hold unspecified values. Two equal objects need not have bytewise-equal representations, and bytewise equality need not express semantic equality.

## Aliasing and alignment

Access memory through a type the object model permits. Misaligned pointers and invalid aliasing are UB even if the hardware tolerates them. Use standard abstractions or carefully reviewed platform APIs for low-level memory.

The permitted alias types include the object's own type and specific character/byte access types, with additional related cases defined by the standard. Violating alias assumptions lets an optimizer treat writes as unable to affect reads through an incompatible type.

Alignment is a requirement on an address for a type. Converting an arbitrary byte address to `T*` does not prove alignment, start `T`'s lifetime, or establish a valid `T` representation.

## Find problems

Enable warnings and run AddressSanitizer and UndefinedBehaviorSanitizer during development. Use ThreadSanitizer where supported for concurrency. See [[Engineering/Warnings, Sanitizers, and Static Analysis]].
