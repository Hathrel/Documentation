# Resource Management and RAII

A resource is anything that must be released: memory, files, sockets, locks, database handles, GPU objects. **RAII** (Resource Acquisition Is Initialization) binds the resource to an object whose destructor releases it.

```cpp
void write_log() {
    std::ofstream file{"app.log"};
    if (!file) throw std::runtime_error{"cannot open app.log"};
    file << "started\n";
} // file closes even if an exception occurs
```

Destructors run automatically when objects leave scope, including during exception unwinding. This gives deterministic cleanup.

## Ownership priority

1. Store the object directly: `Widget widget;`
2. Use a standard resource-owning type: `std::vector`, `std::string`, `std::fstream`, `std::jthread`.
3. Use `std::unique_ptr<T>` when dynamic lifetime or polymorphism is necessary.
4. Use `std::shared_ptr<T>` only for genuine shared lifetime.

```cpp
auto widget = std::make_unique<Widget>(args);
use(*widget);
```

Avoid explicit `new` and `delete` in application code. Smart-pointer factories prevent leaks during construction and make ownership visible.

## Rule of zero and five

If all members manage themselves, declare no destructor/copy/move functions: the **rule of zero**. If a low-level resource wrapper must define one ownership special member, it often needs all five (destructor, copy constructor/assignment, move constructor/assignment)—or explicitly delete copying.

```cpp
class UniqueHandle {
public:
    ~UniqueHandle() { if (handle_) close(handle_); }
    UniqueHandle(const UniqueHandle&) = delete;
    UniqueHandle& operator=(const UniqueHandle&) = delete;
    UniqueHandle(UniqueHandle&& other) noexcept
        : handle_{std::exchange(other.handle_, {})} {}
private:
    NativeHandle handle_{};
};
```

Prefer existing wrappers; hand-written resource classes require careful tests. Destructors should not throw.

See [[Standard Library/Memory and Smart Pointers]].

