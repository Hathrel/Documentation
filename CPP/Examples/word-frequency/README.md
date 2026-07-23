# Word Frequency Example

A complete C++20 command-line program demonstrating file I/O, strings, validation, a hash map, algorithms, lambdas, explicit errors, and CMake.

## Build and run

```sh
cmake -S . -B build
cmake --build build --config Debug
./build/word_frequency input.txt 20
```

With a multi-configuration generator such as Visual Studio, the executable is usually under `build/Debug/` and should be run as:

```powershell
.\build\Debug\word_frequency.exe input.txt 20
```

The optional last argument is the number of results (default 20). Tokenization treats ASCII letters and digits as word characters and lowercases ASCII. It is intentionally not a Unicode tokenizer.

Related: [[Projects/Project Ideas and Exercises]], [[Recipes/Parse Numbers Safely]], [[Standard Library/Algorithms and Ranges]].

