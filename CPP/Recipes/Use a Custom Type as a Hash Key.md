# Use a Custom Type as a Hash Key

An unordered container requires equality and a hash consistent with it: equal keys must have equal hashes.

```cpp
struct Coordinate {
    int x{};
    int y{};
    friend bool operator==(const Coordinate&, const Coordinate&) = default;
};

struct CoordinateHash {
    std::size_t operator()(const Coordinate& value) const noexcept {
        const auto h1 = std::hash<int>{}(value.x);
        const auto h2 = std::hash<int>{}(value.y);
        return h1 ^ (h2 + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
    }
};

std::unordered_map<Coordinate, Tile, CoordinateHash> tiles;
```

This combination is adequate for ordinary in-process tables, not cryptographic/adversarial hashing. Equality and hashing must use the same logical fields and those fields must not change while a key is stored.

A permitted `std::hash<Coordinate>` specialization is possible for a user-defined type, but a named hasher is explicit and supports multiple policies. Never specialize other standard templates unless explicitly allowed.

For ordered containers, define a strict weak ordering with `operator<=>`, `operator<`, or a comparator. `std::tie` is convenient for lexicographic comparison.

