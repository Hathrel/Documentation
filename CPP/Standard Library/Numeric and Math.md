# Numeric and Math

## Math functions

`<cmath>` provides `abs`, `sqrt`, `pow`, trigonometric, exponential, logarithmic, rounding, classification, and remainder functions. Overloads operate on floating-point types.

```cpp
const double hypotenuse = std::hypot(x, y);
if (std::isfinite(value)) { /* ... */ }
```

`<numbers>` (C++20) provides constants such as `std::numbers::pi_v<double>`.

## Numeric algorithms

`<numeric>` includes:

- `std::accumulate`: ordered fold.
- `std::reduce`: may reorder; parallel overloads possible; operation should be associative/commutative as needed.
- `inner_product`, `transform_reduce`.
- `partial_sum`, `inclusive_scan`, `exclusive_scan`.
- `iota`: fill consecutive values.
- `gcd`, `lcm`, `midpoint`.

```cpp
const auto total = std::accumulate(values.begin(), values.end(), std::int64_t{0});
```

The initial value selects the accumulator type.

## Floating point

Floating point includes infinities, NaNs, signed zero, and rounding. Avoid a universal epsilon comparison; tolerance must reflect scale and domain:

```cpp
bool nearly_equal(double a, double b, double abs_tol, double rel_tol) {
    const auto diff = std::abs(a - b);
    return diff <= std::max(abs_tol, rel_tol * std::max(std::abs(a), std::abs(b)));
}
```

For money, exact counters, or decimal regulations, use an integer minor unit or an appropriate decimal/fixed-point library—not binary floating point by default.

## Limits and overflow

`std::numeric_limits<T>` exposes range and representation properties. Validate before overflow because signed overflow cannot be detected reliably afterward. Use checked arithmetic helpers or wider types and prove bounds.

`<complex>`, `<valarray>`, and ratio facilities serve specialized numeric work; serious linear algebra commonly needs a dedicated library.

