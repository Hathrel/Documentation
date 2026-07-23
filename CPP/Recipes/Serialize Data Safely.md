# Serialize Data Safely

Serialization is a defined mapping between domain values and bytes/text. Do not dump object memory.

## Define the format

Specify:

- magic/version;
- byte order and integer widths;
- encoding and string lengths;
- optional/repeated-field representation;
- maximum sizes and nesting;
- checksum/authentication needs;
- forward/backward compatibility rules.

## Encode an unsigned 32-bit big-endian value

```cpp
void append_u32_be(std::vector<std::byte>& out, std::uint32_t value) {
    out.push_back(static_cast<std::byte>((value >> 24) & 0xff));
    out.push_back(static_cast<std::byte>((value >> 16) & 0xff));
    out.push_back(static_cast<std::byte>((value >> 8) & 0xff));
    out.push_back(static_cast<std::byte>(value & 0xff));
}
```

Decoding must check that four bytes remain *before* reading and must validate semantic ranges afterward.

## Use a proven format

JSON is human-friendly but has numeric/encoding/schema caveats. Protocol Buffers, FlatBuffers, MessagePack, CBOR, and others offer different compatibility and performance tradeoffs. These are not in the C++ standard library; use a maintained library.

Parse into temporary values, validate fully, then construct/commit domain objects. Fuzz decoders and cap every length before allocation. A valid checksum detects accidental corruption; authenticity requires a cryptographic construction.

Never use raw struct bytes: padding can be indeterminate, layout and endianness vary, pointers are meaningless, and object representations may not be portable.

