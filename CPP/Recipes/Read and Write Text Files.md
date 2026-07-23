# Read and Write Text Files

## Read all text with a limit

```cpp
std::string read_text(const std::filesystem::path& path, std::uintmax_t max_size) {
    const auto size = std::filesystem::file_size(path);
    if (size > max_size) throw std::runtime_error{"file too large"};

    std::ifstream input{path, std::ios::binary};
    if (!input) throw std::runtime_error{"cannot open input"};

    std::string data(static_cast<std::size_t>(size), '\0');
    input.read(data.data(), static_cast<std::streamsize>(data.size()));
    if (!input && !input.eof()) throw std::runtime_error{"read failure"};
    data.resize(static_cast<std::size_t>(input.gcount()));
    return data;
}
```

For very large inputs, stream/process chunks or lines instead of loading everything.

## Write text

```cpp
void write_text(const std::filesystem::path& path, std::string_view text) {
    std::ofstream output{path, std::ios::binary | std::ios::trunc};
    if (!output) throw std::runtime_error{"cannot open output"};
    output.write(text.data(), static_cast<std::streamsize>(text.size()));
    if (!output) throw std::runtime_error{"write failure"};
}
```

Opening with truncation can destroy existing contents before a later failure. For important data, write a unique temporary alongside the destination, close/check it, then replace the destination with platform-appropriate atomicity/durability guarantees.

Text encoding is part of the file format. UTF-8 is common; do not assume the platform’s default. See [[Standard Library/Files and Filesystem]].

