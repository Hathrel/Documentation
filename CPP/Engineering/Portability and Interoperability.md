# Portability and Interoperability

Portable C++ still interacts with platform compilers, filesystems, encodings, ABIs, and system APIs.

## Portability practices

- Use fixed-width integers only where representation matters; otherwise use natural domain types.
- Never assume `char` signedness, byte order, struct padding, `long` width, or path syntax.
- Use `std::filesystem::path` for paths and binary mode for defined byte formats.
- Test supported compilers and operating systems in CI.
- Isolate platform code behind small interfaces in separate source files.
- Use feature-test macros/capability checks, not guessed version arithmetic.

## Conditional compilation

```cpp
#if defined(_WIN32)
// Windows implementation
#elif defined(__linux__)
// Linux implementation
#else
#error Unsupported platform
#endif
```

Keep conditionals at integration seams; avoid scattering them through business logic.

## Calling C

```cpp
extern "C" int library_do_work(library_handle*, const char*, std::size_t);
```

C linkage affects name linkage, not type safety or ABI magic. Match the exact C declaration and calling conventions from its header. Wrap handles with RAII and translate error codes to your project policy.

## Exported C++ APIs

If consumers may use different toolchains, expose opaque C handles or a wire format. If the toolchain is controlled, document compiler, standard library, runtime linkage, build configuration, exception/allocator boundary rules, and symbol visibility.

Never let exceptions cross a C boundary; catch and translate them.

