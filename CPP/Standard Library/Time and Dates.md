# Time and Dates

`<chrono>` makes units part of types.

## Durations

```cpp
using namespace std::chrono_literals;
auto timeout = 250ms;
auto total = 2s + timeout;
auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(total);
```

Avoid storing unitless integers named `timeout`. Accept/return duration types and convert only at boundaries.

## Clocks

- `steady_clock`: monotonic; durations, benchmarks, timeouts.
- `system_clock`: wall clock; converts to/from calendar/time-point representations and may jump.
- `high_resolution_clock`: implementation alias; not guaranteed steady.

```cpp
const auto start = std::chrono::steady_clock::now();
do_work();
const auto elapsed = std::chrono::steady_clock::now() - start;
```

## Calendar and time zones (C++20)

Calendar types include `year`, `month`, `day`, `year_month_day`, and `sys_days`. Chrono also standardizes time-zone support, but library implementation availability may lag.

```cpp
using namespace std::chrono;
const year_month_day date{2026y, July, 23d};
if (!date.ok()) report_invalid();
```

Civil time is political and complex: daylight-saving transitions create nonexistent or ambiguous local times. Store instants in a clear global representation, retain the relevant zone when future civil intent matters, and convert for display.

For sleeping, `std::this_thread::sleep_for` and `sleep_until` are minimum-duration waits; scheduling may make them longer.

