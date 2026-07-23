# Command-Line Arguments

The hosted form of `main` receives an argument count and array of C strings:

```cpp
#include <iostream>
#include <string_view>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>\n";
        return 2;
    }

    const std::string_view filename{argv[1]};
    std::cout << "processing " << filename << '\n';
}
```

`argc` is nonnegative. Valid entries are `argv[0]` through `argv[argc - 1]`; `argv[argc]` is null. Argument encoding and quoting rules are platform-dependent, especially on Windows.

## Small option parser

```cpp
struct Options {
    bool verbose{};
    std::string input;
};

std::optional<Options> parse(int argc, char* argv[]) {
    Options out;
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg{argv[i]};
        if (arg == "-v" || arg == "--verbose") {
            out.verbose = true;
        } else if (arg == "-i" || arg == "--input") {
            if (++i == argc) return std::nullopt;
            out.input = argv[i];
        } else {
            return std::nullopt;
        }
    }
    if (out.input.empty()) return std::nullopt;
    return out;
}
```

For subcommands, generated help, combined flags, response files, or platform-aware Unicode, adopt a maintained argument-parsing library.

Return `0` for success and a documented nonzero code for errors. `EXIT_SUCCESS` and `EXIT_FAILURE` from `<cstdlib>` are portable general results.

