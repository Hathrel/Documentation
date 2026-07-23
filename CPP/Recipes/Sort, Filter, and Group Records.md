# Sort, Filter, and Group Records

```cpp
struct Sale {
    std::string product;
    std::string region;
    std::int64_t cents{};
};
```

## Sort by multiple fields

```cpp
std::ranges::sort(sales, [](const Sale& a, const Sale& b) {
    return std::tie(a.region, a.product) < std::tie(b.region, b.product);
});
```

Use `stable_sort` when equivalent records must retain their previous relative order.

## Filter into an owning result

```cpp
std::vector<Sale> expensive;
std::ranges::copy_if(sales, std::back_inserter(expensive),
                     [](const Sale& s) { return s.cents >= 10'000; });
```

Or make a lazy, non-owning view:

```cpp
auto east = sales | std::views::filter([](const Sale& s) { return s.region == "east"; });
```

## Group and aggregate

```cpp
std::unordered_map<std::string, std::int64_t> totals;
for (const auto& sale : sales) {
    totals[sale.region] += sale.cents;
}
```

For deterministic sorted output, copy to a vector and sort, or use `std::map`. Do not assume unordered-map iteration order.

Choose accumulator types that cannot overflow for permitted inputs, and validate limits at ingestion.

