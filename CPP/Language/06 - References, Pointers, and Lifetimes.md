# References, Pointers, and Lifetimes

Most serious C++ bugs are lifetime or ownership bugs. Ask of every pointer-like value: **what object does it refer to, who owns that object, and can it outlive this use?**

## Objects, identity, and borrowing

An object occupies storage during a lifetime and has a type. References, pointers, iterators, spans, and views can designate an object without owning it. They are **borrows**: destroying or relocating the owner can invalidate them while their own bit patterns remain unchanged.

C++ does not track borrow validity at runtime. A non-null address can still be dangling, misaligned, one-past-the-end, or a pointer to storage where the expected object no longer exists. Validity comes from program structure and API contracts.

## References

A reference is an alias for an existing object. It must be initialized and normally cannot be reseated.

```cpp
int value{10};
int& mutable_ref = value;
const int& view = value;
mutable_ref = 20;          // changes value
```

Use `T&` for a required, non-owning, mutable parameter and `const T&` for a required, non-owning, read-only parameter when copying would be undesirable.

A reference is not an independently reseatable handle. Assigning through it assigns the referred object:

```cpp
int first{1};
int second{2};
int& ref = first;
ref = second; // first becomes 2; ref still refers to first
```

Reference types are not objects: there are no arrays of references, pointers to references, or references to references after type formation. Template deduction can nevertheless produce forms such as `T& &`; **reference collapsing** reduces any combination containing an lvalue reference to `T&`, while `T&& &&` becomes `T&&`.

### Binding rules

A mutable lvalue reference normally binds only to a compatible lvalue. A const lvalue reference can also bind to a temporary. An rvalue reference binds to an rvalue and signals that the referred object may be consumed:

```cpp
int n{};
int& a = n;
const int& b = 42;        // temporary lifetime is extended
std::string&& c = make(); // named c is itself an lvalue expression
```

Reference binding can extend some temporary lifetimes, but the rules are syntax-sensitive and do not “pass through” arbitrary functions. A reference returned by a function is only as valid as the object it denotes.

References are usually implemented as addresses, but the language specifies aliasing behavior rather than representation. A reference must designate a valid object or function when used; there is no standard null-reference state.

## Pointers

A pointer stores an address and may be null.

```cpp
int value{10};
int* ptr = &value;
if (ptr != nullptr) {
    *ptr = 20;             // dereference
}
```

Use a raw pointer primarily as an optional, non-owning observer. Express owning memory with values, containers, or smart pointers from [[Standard Library/Memory and Smart Pointers]]. Prefer `nullptr` to `0` or `NULL`.

The pointer type states the type reached by dereference:

```cpp
Widget* p = &widget;          // pointer to Widget
const Widget* view = p;       // may not modify Widget through view
Widget** out = &p;            // pointer to a pointer
void* raw = p;                // object address without a pointed-to type
```

`&object` takes an address; `*pointer` dereferences; `pointer->member` is shorthand for `(*pointer).member`. Dereference requires a pointer that designates a live, suitably aligned object or function of a compatible type. Checking only against `nullptr` does not establish those other facts.

### Pointer states

- A **null pointer** deliberately points to no object; compare it with `nullptr` and do not dereference it.
- An **uninitialized pointer** has an indeterminate value and must not be read.
- A **dangling pointer** retained an address after the target's lifetime ended or storage moved.
- A **one-past pointer** can mark the end of an array range and be compared or subtracted appropriately, but not dereferenced.
- A valid pointer may still be non-owning; `delete` is correct only for a matching allocation owned by that code.

Initialize pointer variables, make nullability explicit in the API, and prefer a reference when absence is not meaningful.

### Pointer conversions and const

`T*` converts to `const T*` because granting read-only access is safe. The reverse requires a cast and does not permit modifying an originally const object. The levels matter:

```cpp
const int* p;       // pointer to const int
int* const p2 = &n; // const pointer to mutable int
const int* const p3 = &n;
```

Converting `Derived*` to an accessible, unambiguous `Base*` adjusts the pointer if necessary. Converting back requires knowledge of the dynamic object; use virtual dispatch or a checked `dynamic_cast` when appropriate.

An object pointer converts to `void*` and back to the original type, but `void*` cannot be dereferenced or used in standard pointer arithmetic. Function pointers and pointers-to-members are separate categories with different syntax and guarantees; they are not portable object addresses.

## Lifetimes

Lifetime is not the same as scope or storage duration. Scope concerns a name; storage duration concerns memory; lifetime is the interval during which an object of a particular type exists in that storage. A pointer may remain in scope after its target's lifetime ends.

Automatic objects are destroyed when their scope exits. Members are destroyed after the destructor body, in reverse declaration order; bases follow in reverse construction order. Dynamic objects are destroyed by the corresponding owning operation. Placement construction and unions can begin and end lifetimes explicitly, but such code must obey the object-model rules in [[Language/20 - Undefined Behavior and the Object Model]].

```cpp
const std::string& bad() {
    std::string local{"gone soon"};
    return local; // dangling reference after return: undefined behavior
}
```

Common dangling cases:

- returning a pointer/reference/view to a local object;
- retaining a pointer after its owner is destroyed;
- keeping a `std::string_view` after the string dies or changes;
- using vector iterators/references after an operation that invalidates them;
- capturing a local by reference in a lambda that outlives the local.

### Temporary lifetime

Most temporaries are destroyed at the end of the full expression. Directly binding certain references extends a temporary's lifetime:

```cpp
const std::string& text = std::string{"safe until text leaves scope"};
```

The extension is not generally transferred:

```cpp
const std::string& identity(const std::string& value) { return value; }
const std::string& bad = identity(std::string{"dies after this statement"});
```

Returning a reference bound to a temporary does not extend it. A reference member initialized from a constructor parameter similarly depends on the caller's object.

### Invalidation

An owner can remain alive while a borrow becomes invalid:

- vector growth may relocate all elements;
- string mutation may replace its buffer;
- erasure ends the erased element's lifetime;
- moving an object may transfer the resource a view referred into;
- replacing an `optional` or changing a `variant` alternative destroys the old contained object.

Consult each type's invalidation contract. “The owner still exists” is necessary but not sufficient.

## Arrays and pointer arithmetic

Built-in arrays often decay to pointers and lose size information. Prefer `std::array`, `std::vector`, or `std::span`. Pointer arithmetic is valid only within one array (plus its one-past-end position).

Given `T array[N]`, most expressions convert `array` to a pointer to its first element. Important exceptions include `sizeof(array)`, `&array`, and binding to an array reference. The resulting pointer does not carry `N`.

`array + i` and `&array[i]` are equivalent when `i` is in range or one-past. Subtracting two pointers is defined only when they refer into the same array object (including one-past); the result type is `std::ptrdiff_t`. Relational comparison of unrelated pointers is not generally a portable address ordering.

Never use a sentinel alone when the API can carry a size. Prefer `std::span<T>` for a borrowed contiguous range:

```cpp
void fill(std::span<int> output);
void inspect(std::span<const int> input);
```

A span does not own or extend the lifetime of the elements.

## Dynamic allocation and ownership

`new T(args)` obtains storage and constructs a `T`; `delete p` destroys that object and releases matching storage. Array forms `new T[n]` and `delete[]` must match. Mismatching forms, deleting twice, deleting a non-owned pointer, or losing the last owning address is invalid.

Application code should rarely spell these operations. Containers and smart pointers pair acquisition and release through RAII and remain correct across exceptions:

```cpp
auto widget = std::make_unique<Widget>(args);
Widget* observer = widget.get(); // non-owning; valid only while widget owns it
```

Pointer syntax does not encode ownership. A raw pointer can point into the stack, a container, a static object, memory owned elsewhere, or a dynamic allocation. State the ownership contract in the type and API.

## Ownership vocabulary

- `T`: contained value; owns its resources.
- `T&`: required non-owning access.
- `T*`: optional non-owning access (by convention).
- `std::unique_ptr<T>`: exclusive ownership.
- `std::shared_ptr<T>`: shared lifetime; use sparingly.
- `std::weak_ptr<T>`: non-owning observation of shared ownership.

See [[Language/11 - Resource Management and RAII]] and [[Language/12 - Copying, Moving, and Value Categories]].

## API design checklist

For every borrowed return value or stored borrow, answer:

1. Which object owns the target?
2. Can null or emptiness occur?
3. Which operations invalidate the borrow?
4. Can the borrow escape the owner's scope or cross a thread boundary?
5. Does the function need one object, an optional object, or a range?

Prefer `T&` for one required object, `T*` for an optional object when pointer syntax is appropriate, and `std::span<T>` or `std::string_view` for ranges. Use a smart pointer parameter only when the function participates in ownership—not merely to prove the caller happens to use a smart pointer.
