# Input and Output

Streams model formatted and unformatted I/O. `std::cin`, `std::cout`, and `std::cerr` are standard input, output, and error.

## Formatted I/O

```cpp
int count{};
if (std::cin >> count) {
    std::cout << "count = " << count << '\n';
} else {
    std::cerr << "expected an integer\n";
}
```

Extraction returns the stream, whose boolean state reports success. Do not use `while (!stream.eof())`; attempt the read:

```cpp
Record record;
while (input >> record) {
    records.push_back(record);
}
if (!input.eof()) report_malformed_input();
```

## Formatting state

`<iomanip>` includes `std::fixed`, `std::scientific`, `std::setprecision`, `std::setw`, `std::setfill`, `std::hex`, and `std::quoted`. Many manipulators persist on the stream; width usually affects only the next field.

```cpp
std::cout << std::fixed << std::setprecision(2) << price << '\n';
```

## Lines and string streams

Use `std::getline` for line-oriented input. `std::istringstream` parses from a string and `std::ostringstream` builds a string, though `std::format`/`to_chars` may be clearer or faster for targeted jobs.

## Flushing and performance

`'\n'` writes a newline; `std::endl` also flushes. Flush when interactive/protocol behavior needs it, not after every line. `std::ios::sync_with_stdio(false); std::cin.tie(nullptr);` may improve console throughput when you do not mix C and C++ I/O and do not need automatic pre-input flushing.

Streams can be configured to throw on selected state errors, but state checking is the traditional style. See [[Standard Library/Files and Filesystem]].

