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

RAII is broader than memory management. Construction establishes a class invariant that includes ownership; destruction ends that ownership. The resource need not be acquired in the constructor itself, but every successful object state must have a deterministic release path.

## Stack unwinding and destructor rules

When an exception propagates, fully constructed automatic objects are destroyed in reverse construction order. An object whose constructor throws is not itself destroyed, but its already-constructed bases and members are. Acquire resources into RAII members as early as possible so partial construction cleans itself up.

Destructors are implicitly `noexcept` when their members and bases permit it. If a destructor throws while another exception is already unwinding, the program terminates. Release functions used by destructors should therefore report failures through logging, explicit earlier `close()` operations, or another non-throwing policy.

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

## Unique and shared ownership

`std::unique_ptr<T>` models one transferable owner. Moving transfers ownership and leaves the source empty. Use `std::make_unique` for construction. A custom deleter adapts handles that need a special release operation and becomes part of the unique pointer's type.

`std::shared_ptr<T>` stores shared ownership through a control block. Copying increments a reference count; destruction occurs when the last owning reference disappears. This adds allocation, atomic bookkeeping in typical implementations, and less-local lifetime reasoning. It should represent genuine shared lifetime, not uncertainty about the owner.

A cycle of shared pointers never reaches zero. Use `std::weak_ptr` for non-owning edges such as parent links, observers, and caches. Locking a weak pointer produces a temporary shared owner only if the object still exists.

Do not construct multiple independent shared pointers from the same raw pointer. Prefer `std::make_shared`, and use `std::enable_shared_from_this` only when an object already managed by shared ownership must safely produce another owner.

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

A correct exclusive wrapper also needs move assignment that first releases its current resource, handles self-move safely, and steals the source. A `swap`-based implementation can simplify this. The moved-from wrapper must remain destructible and assignable; an empty sentinel state is usually best.

Copying a resource wrapper must mean either a genuine independent duplication operation or be deleted. Copying the numeric value of a file descriptor, socket, or native handle usually creates two apparent owners and a double close.

## Exception safety

Common guarantees are:

- **no-throw:** the operation cannot fail by throwing;
- **strong:** on failure, externally visible state is unchanged;
- **basic:** invariants hold and resources do not leak, but values may change;
- **no guarantee:** even invariants may be lost.

Build new state in temporary RAII objects, then commit with a non-throwing swap or move. This “do work, then commit” structure often provides the strong guarantee naturally.

Locks are resources too. `std::lock_guard` and `std::unique_lock` release mutexes on every exit path. Scope guards generalize “run this action at scope exit” for APIs that have acquire/release pairs but no dedicated wrapper.

See [[Standard Library/Memory and Smart Pointers]].
