# Files and Filesystem

## Text files

```cpp
#include <fstream>
#include <string>

std::ifstream input{"data.txt"};
if (!input) throw std::runtime_error{"could not open data.txt"};

for (std::string line; std::getline(input, line); ) {
    process(line);
}
if (input.bad()) throw std::runtime_error{"I/O failure"};
```

`std::ofstream` writes (truncating by default), and `std::fstream` reads/writes. Use `std::ios::app` to append and `std::ios::binary` where text translation is unwanted.

## Binary files

Read/write byte buffers, not raw structs with platform-dependent padding and endianness.

```cpp
std::vector<std::byte> bytes(size);
input.read(reinterpret_cast<char*>(bytes.data()),
           static_cast<std::streamsize>(bytes.size()));
```

Check the number read with `gcount()` where partial reads matter. Define file formats explicitly, validate sizes before allocation, and distrust all external data.

## Filesystem (C++17)

```cpp
#include <filesystem>
namespace fs = std::filesystem;

fs::path root{"data"};
for (const fs::directory_entry& entry : fs::directory_iterator{root}) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
        std::cout << entry.path() << '\n';
    }
}
```

Useful operations: `exists`, `is_regular_file`, `file_size`, `create_directories`, `copy`, `rename`, `remove`, `canonical`, `relative`, `current_path`, and directory iterators.

Filesystem functions often offer both throwing overloads and `std::error_code&` overloads. Use error-code overloads when individual failures are expected and must not throw.

## Safe writes

For important data, write to a temporary file in the same directory, flush/close and verify it, then replace/rename according to platform guarantees. Validate user-derived paths before destructive operations; lexical normalization alone does not establish security.

