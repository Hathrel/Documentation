# Object-Oriented Programming

C++ supports object-oriented programming, but not every abstraction needs inheritance. Prefer values and composition; use runtime polymorphism when objects with a shared interface must vary dynamically.

## Runtime polymorphism

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    [[nodiscard]] virtual double area() const = 0;
};

class Circle final : public Shape {
public:
    explicit Circle(double radius) : radius_{radius} {}
    double area() const override { return std::numbers::pi * radius_ * radius_; }
private:
    double radius_;
};

std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(2.0));
```

A base used polymorphically needs a public virtual destructor. Use `override` on overrides and `final` when extension is intentionally closed. Pure virtual `= 0` makes an abstract interface.

Virtual dispatch occurs when a virtual function is called through a base pointer or reference. Calls on an object by value use its static object, and qualified calls can suppress virtual dispatch.

During base construction and destruction, virtual calls dispatch only within the portion currently being constructed or destroyed—not to a more-derived override. Do not build designs that require derived behavior from base constructors or destructors.

An override must match the base signature, including cv/ref qualifiers. `override` asks the compiler to verify that match and prevents accidental hiding. A derived declaration with the same name can hide all base overloads; `using Base::function;` can reintroduce them.

## Virtual destructors and ownership

Deleting a derived object through a base pointer requires a virtual base destructor. An interface can use a public virtual destructor for polymorphic deletion, or a protected non-virtual destructor when deletion through the interface is deliberately forbidden.

`std::unique_ptr<Base>` is the default owning polymorphic handle. Copying such objects requires an explicit policy, often a virtual `clone()` returning a unique pointer.

## Avoid slicing

Passing or storing a derived object by base **value** removes its derived part. Use references/pointers for polymorphism and smart pointers for ownership.

Slicing also occurs when a derived object is passed to a by-value base parameter or assigned into an existing base object. Containers of base values cannot preserve heterogeneous dynamic types.

## Inheritance relationships

Public inheritance promises substitutability: code correct for the base should remain correct for the derived type. Protected and private inheritance change accessibility and usually model implementation rather than an external “is-a” relationship.

Base subobjects are initialized before derived members and destroyed afterward. A derived object may contain more than one base subobject; pointer adjustment is therefore part of base conversions and object layout.

## Composition first

Inheritance means “is substitutable for,” not merely “reuses implementation.” A car has an engine, so composition is natural. Small components are easier to test and change.

## Other polymorphism

- Templates/concepts: compile-time polymorphism, no runtime dispatch.
- `std::variant`: a closed set of alternatives with explicit visitation.
- Type erasure (`std::function`, custom wrappers): runtime polymorphism without exposing inheritance.

Use the simplest mechanism matching whether the set of types is open/closed and known at compile time/runtime.

Runtime inheritance supports an open set of derived types but brings indirection, lifetime management, and ABI coupling. A variant supports a closed set and makes every alternative visible to visitors. Templates avoid runtime dispatch but require implementations and concrete types at compile time. Type erasure can preserve value-like APIs while hiding the concrete implementation.

## RTTI and casts

`dynamic_cast` requires a polymorphic source type. A failed pointer downcast returns null; a failed reference downcast throws `std::bad_cast`. `typeid` observes dynamic type through a polymorphic glvalue, with special behavior for null dereference expressions.

Frequent downcasting often means the base interface lacks an operation or the abstraction should be a variant. Use RTTI when runtime type inquiry is genuinely part of the problem, not to reconstruct manual virtual dispatch.

## Multiple inheritance

It is valid and can model multiple pure interfaces, but stateful multiple inheritance and virtual inheritance complicate layout, construction, and reasoning. Avoid unless the design clearly demands it.
