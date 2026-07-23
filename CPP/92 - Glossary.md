# Glossary

**ABI (Application Binary Interface)** — Binary-level calling, layout, symbol, and runtime conventions between compiled components.

**abstract class** — Class that cannot be directly instantiated, usually because it has a pure virtual function.

**algorithm** — Operation over ranges, such as searching, sorting, or transforming.

**allocation** — Obtaining storage. Object lifetime and storage duration are related but distinct concepts.

**argument / parameter** — An argument is supplied at a call; a parameter is the function’s local declaration receiving it.

**automatic storage duration** — Storage for most local objects, released automatically at block exit.

**brace initialization** — Initialization with `{}`; supports aggregate initialization and rejects many narrowing conversions.

**class invariant** — Condition that valid observable instances maintain between public operations.

**compile time / runtime** — Translation of source versus execution of the resulting program.

**concept** — Named compile-time requirements used to constrain templates.

**const correctness** — Accurately expressing and enforcing which operations do not mutate observable state.

**constructor / destructor** — Special functions establishing an object and cleaning it up at lifetime end.

**copy / move** — Duplicate a value versus transfer resources from an expiring source.

**data race** — Unsynchronized conflicting concurrent accesses; undefined behavior.

**declaration / definition** — Introduces a name/type versus supplies the entity/body/storage.

**dynamic allocation** — Storage requested and released at runtime, preferably managed by an owning RAII type.

**encapsulation** — Hiding representation behind operations that preserve an abstraction.

**exception** — Object thrown to transfer control to a matching handler while stack objects are unwound.

**expression / statement** — An expression computes a value and/or side effect; a statement is an executable language unit.

**header / source file** — Header is typically included for declarations/templates; source file is compiled as a translation unit.

**implementation-defined behavior** — Implementation chooses and documents one allowed behavior.

**include guard** — Preprocessor pattern preventing repeated header inclusion in one translation unit.

**iterator / sentinel** — Iterator denotes a range position; sentinel marks an end, possibly with another type.

**lambda** — Expression creating an unnamed callable object, optionally capturing surrounding state.

**linker** — Combines object files/libraries and resolves externally linked names.

**lifetime** — Period during which an object exists and may be used under the language rules.

**lvalue / rvalue** — Practical distinction between persistent identity and value eligible for temporary/move use; see [[Language/12 - Copying, Moving, and Value Categories]].

**namespace** — Named scope organizing declarations and preventing collisions.

**narrowing conversion** — Conversion that may lose range or precision.

**ODR (One Definition Rule)** — Rules governing allowed definitions of entities across a program.

**overload** — One of multiple functions/operators sharing a name with distinguishable parameter signatures.

**ownership** — Responsibility for ending a resource’s lifetime/releasing it.

**polymorphism** — One interface supporting different types/behaviors at compile time or runtime.

**precondition / postcondition** — What must hold before an operation / what it guarantees afterward.

**preprocessor** — Textual phase processing directives such as `#include`, `#if`, and macros.

**RAII** — Resource Acquisition Is Initialization; scope-bound objects release resources in destructors.

**reference** — Alias to an existing object; does not independently own it.

**scope** — Region where a name is visible.

**smart pointer** — RAII class modeling dynamic ownership (`unique_ptr`, `shared_ptr`) or shared observation (`weak_ptr`).

**static storage duration** — Storage lasting for the program, including namespace-scope objects and function-local statics.

**template** — Pattern for generating functions/classes/variables from type or value arguments.

**thread safety** — Documented guarantees for concurrent use of an object/API.

**translation unit** — One source file after preprocessing, compiled independently.

**type deduction** — Compiler inference of a type, through `auto`, templates, structured bindings, etc.

**undefined behavior** — No requirements imposed by the C++ standard after a rule violation.

**unspecified behavior** — Implementation may choose among allowed outcomes without documenting which.

**value semantics** — Objects behave as independent values under copy/assignment/comparison, simplifying ownership.

**view** — Usually a lightweight non-owning representation of another object/range; lifetime must be managed externally.

**virtual function** — Member dispatched at runtime through a polymorphic base reference/pointer.

