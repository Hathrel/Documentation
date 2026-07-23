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

## Avoid slicing

Passing or storing a derived object by base **value** removes its derived part. Use references/pointers for polymorphism and smart pointers for ownership.

## Composition first

Inheritance means “is substitutable for,” not merely “reuses implementation.” A car has an engine, so composition is natural. Small components are easier to test and change.

## Other polymorphism

- Templates/concepts: compile-time polymorphism, no runtime dispatch.
- `std::variant`: a closed set of alternatives with explicit visitation.
- Type erasure (`std::function`, custom wrappers): runtime polymorphism without exposing inheritance.

Use the simplest mechanism matching whether the set of types is open/closed and known at compile time/runtime.

## Multiple inheritance

It is valid and can model multiple pure interfaces, but stateful multiple inheritance and virtual inheritance complicate layout, construction, and reasoning. Avoid unless the design clearly demands it.

