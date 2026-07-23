# C++ Field Guide

Welcome. This vault assumes you have never written C++.

## Start here

1. Read [[01 - Learning Path]].
2. Set up your system with [[02 - Development Environment]].
3. Compile [[03 - Your First Program]].
4. Work through the numbered notes in **Language**.
5. Build the exercises in [[Projects/Project Ideas and Exercises]].

## Maps of content

- [[Language/Language Map|Language and syntax]]
- [[Standard Library/Standard Library Map|Standard library]]
- [[Engineering/Engineering Map|Building reliable software]]
- [[Recipes/Recipes Map|How-to recipes]]
- [[Projects/Project Ideas and Exercises|Projects and exercises]]
- [[90 - C++ Cheat Sheet]]
- [[91 - Standard Library Quick Reference]]
- [[92 - Glossary]]
- [[93 - Further Reading]]
- [[94 - C++ Versions and Compatibility]]

## What “modern C++” means here

The default is **C++20**. Prefer values, scoped ownership, standard containers, algorithms, RAII, and clear types. Avoid manual memory management, C-style arrays, macros, and inheritance unless they solve a real problem. Features requiring C++23 are marked **C++23**.

## In-depth language reference

For exhaustive C++ language details, standard-library APIs, overloads, requirements, complexity guarantees, and compiler-support information, open [cppreference](https://en.cppreference.com/). This external link opens the reference website in your browser.

> [!warning] C++ gives you control
> That control includes the ability to write programs with undefined behavior. Compile with warnings, test, and use sanitizers. See [[Engineering/Warnings, Sanitizers, and Static Analysis]].

## A tiny complete program

```cpp
#include <iostream>
#include <string>

int main() {
    const std::string name{"world"};
    std::cout << "Hello, " << name << "!\n";
    return 0;
}
```

See [[03 - Your First Program]] for a line-by-line explanation.
