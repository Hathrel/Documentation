# Implement Operators and Hashing

Operators should preserve the meaning users expect from the type.

## Equality and ordering

```cpp
struct Version {
    int major{};
    int minor{};
    int patch{};

    auto operator<=>(const Version&) const = default;
};
```

C++20 defaulted three-way comparison generates lexicographic ordering and equality when members support it. Do not define an ordering that contradicts equality.

## Arithmetic type

```cpp
class Meters {
public:
    explicit constexpr Meters(double value) : value_{value} {}
    constexpr Meters& operator+=(Meters rhs) {
        value_ += rhs.value_;
        return *this;
    }
    friend constexpr Meters operator+(Meters lhs, Meters rhs) {
        lhs += rhs;
        return lhs;
    }
private:
    double value_{};
};
```

Implement compound assignment as the primitive and binary arithmetic in terms of it. Pass the left operand by value so moves/copies work naturally. Respect algebraic expectations where applicable.

## Stream output

```cpp
std::ostream& operator<<(std::ostream& out, const Version& v) {
    return out << v.major << '.' << v.minor << '.' << v.patch;
}
```

Put non-member operators in the same namespace as the type so argument-dependent lookup finds them. Avoid surprising overloads, implicit conversions, and global operators for types you do not own.

For hashing, see [[Recipes/Use a Custom Type as a Hash Key]].

