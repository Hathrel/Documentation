# Type Support and Low-Level Utilities

These headers support generic, systems, and interoperability code.

## Types and limits

- `<cstdint>`: fixed/minimum-width integer types.
- `<cstddef>`: `size_t`, `ptrdiff_t`, `byte`, `nullptr_t`.
- `<limits>`: numeric properties.
- `<type_traits>` and `<concepts>`: compile-time type properties/requirements.
- `<typeinfo>` / `std::type_index`: runtime type information for polymorphic types.

## Bits and bytes

`<bit>` (mostly C++20) includes `bit_cast`, endian inspection, rotations, bit counts, powers-of-two helpers.

```cpp
float value{1.0F};
const auto bits = std::bit_cast<std::uint32_t>(value);
if constexpr (std::endian::native == std::endian::little) { /* ... */ }
```

Endianness inspection does not serialize a value for you. Define byte order and encode/decode fields explicitly.

## C interoperability

Use `extern "C"` for C linkage where required. C APIs usually communicate through raw pointers, lengths, handles, and error codes. Wrap them immediately in type-safe C++ RAII interfaces.

```cpp
using FilePtr = std::unique_ptr<std::FILE, decltype(&std::fclose)>;
FilePtr file{std::fopen(path, "rb"), &std::fclose};
```

Check the API’s exact ownership, nullability, threading, and encoding contract.

## Alignment and raw storage

`alignof`, `std::align`, aligned allocation, placement construction, and lifetime-start utilities exist for allocators and systems work. Their rules are unforgiving. Prefer containers, `std::optional`, `std::variant`, and smart pointers unless implementing a low-level abstraction with tests and sanitizer coverage.

See [[Language/20 - Undefined Behavior and the Object Model]].

