# Further Reading

Use this vault for explanation and navigation. Use current authoritative references for exact overloads, defect reports, implementation status, and evolving standards.

## References

- [cppreference](https://en.cppreference.com/) — detailed language and library reference with examples and implementation-status tables.
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — safety- and design-oriented guidelines maintained by the C++ community.
- [C++ FAQ](https://isocpp.org/faq) — explanations from the Standard C++ Foundation.
- [Compiler Explorer](https://godbolt.org/) — compare compiler diagnostics and generated assembly without local setup.
- [CMake documentation](https://cmake.org/cmake/help/latest/) — authoritative CMake command and workflow reference.

## Books by stage

- Beginner: *Programming: Principles and Practice Using C++* (Stroustrup) or a current beginner text aligned with modern C++.
- Fast language overview for experienced programmers: *A Tour of C++* (Stroustrup).
- Good habits and design: *Effective Modern C++* (Meyers; C++11/14 but many principles endure), supplemented with current guidance.
- Standard library: *C++ Standard Library* (Josuttis; match edition to language version).
- Concurrency: *C++ Concurrency in Action* (Williams; match edition to standard version).

Check editions and the language version they cover. Older C++ material commonly teaches manual memory, C arrays, macros, and outdated idioms as defaults.

## How to read a reference page

1. Confirm the feature’s standard version and header.
2. Identify the exact overload and template requirements.
3. Read preconditions and undefined-behavior clauses.
4. Check complexity and iterator/reference invalidation.
5. Check exception and thread-safety notes.
6. Compile a minimal example on every supported toolchain.

The ISO standard is the normative source, but it is written for implementers and experts rather than as a tutorial.

