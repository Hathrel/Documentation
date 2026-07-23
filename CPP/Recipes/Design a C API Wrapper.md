# Design a C API Wrapper

Wrap a C handle immediately so cleanup occurs on every path.

Suppose a C API supplies `device_open`, `device_close`, and `device_read`:

```cpp
struct DeviceCloser {
    void operator()(device* p) const noexcept {
        if (p) device_close(p);
    }
};

class Device {
public:
    explicit Device(std::string_view name) {
        std::string terminated{name};
        handle_.reset(device_open(terminated.c_str()));
        if (!handle_) throw std::runtime_error{"device_open failed"};
    }

    std::size_t read(std::span<std::byte> buffer) {
        const auto result = device_read(handle_.get(), buffer.data(), buffer.size());
        if (result < 0) throw DeviceError{device_last_error(handle_.get())};
        return static_cast<std::size_t>(result);
    }

private:
    std::unique_ptr<device, DeviceCloser> handle_;
};
```

The wrapper is automatically move-only because of `unique_ptr`. It converts error conventions, preserves null termination, and couples buffers with sizes.

Before wrapping any API, determine:

- who owns each pointer and how it is freed;
- whether null is allowed;
- length units and encoding;
- whether partial reads/writes occur;
- thread-safety and callback lifetime;
- error retrieval timing;
- whether callbacks can throw (they must not cross C).

Catch all exceptions in functions called by C and translate to the documented error result.

