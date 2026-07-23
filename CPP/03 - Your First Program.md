# Your First Program

Create `main.cpp`:

```cpp
#include <iostream>
#include <string>

int main() {
    std::cout << "What is your name? ";

    std::string name;
    std::getline(std::cin, name);

    std::cout << "Hello, " << name << "!\n";
    return 0;
}
```

Compile it using [[02 - Development Environment]].

## Line by line

- `#include <iostream>` makes stream input/output declarations available.
- `#include <string>` provides `std::string` and `std::getline`.
- `int main()` is where a hosted C++ program starts. Its integer result is returned to the operating system; zero means success.
- `{` and `}` delimit a block.
- `std::cout` is standard output; `<<` sends values to it.
- `std::string name;` creates an initially empty string.
- `std::getline(std::cin, name)` reads one entire line from standard input.
- `;` ends most statements.
- `\n` is a newline character. Prefer it to `std::endl` unless you specifically need to flush output.

`std::` identifies names in the standard namespace. Avoid `using namespace std;`, especially in headers: it creates ambiguity as programs grow.

## First experiments

1. Print two lines.
2. Ask for age with `int age{}; std::cin >> age;`.
3. Print the age next year.
4. Enter text after numeric input and observe how formatted input interacts with `getline`; fix it using `std::getline(std::cin >> std::ws, name)`.

Next: [[Language/01 - Program Structure and Compilation]].

