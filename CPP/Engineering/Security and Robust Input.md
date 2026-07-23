# Security and Robust Input

Treat every external byte, length, count, path, and identifier as untrusted.

## Input checklist

1. Limit total input size before allocating.
2. Parse into sufficiently wide types without overflow.
3. Validate syntax, range, relationships, and duplicates.
4. Reject trailing/ambiguous input unless explicitly allowed.
5. Avoid partial state changes; parse then commit.
6. Bound recursion, loops, decompression, and resource use.
7. Report errors without leaking secrets.

Use `std::from_chars` for controlled numeric parsing, `.at()` or checked logic for boundaries, and explicit serialization. Beware multiplying sizes before checking overflow.

## Memory safety

Prefer values, containers, spans, and RAII. Use sanitizers, fuzz parsers, and keep unsafe pointer arithmetic isolated and reviewed. Integer overflow can become buffer under-allocation.

## Injection and paths

Do not build shell commands, SQL, or structured formats by string concatenation. Use argument arrays, parameterized queries, and encoders. For filesystem access, define an allowed root and validate the resolved path according to platform and symlink/security requirements before writing or deleting.

## Secrets and crypto

Do not log credentials or tokens. Standard strings do not guarantee secure erasure. Do not design cryptographic algorithms or use standard PRNGs for secrets; use vetted, current platform/cryptographic libraries.

## Denial of service

Hash tables, regexes, parsers, concurrency queues, and caches can be driven into excessive CPU/memory use. Apply quotas, timeouts, depth limits, cancellation, and backpressure.

Security requirements change with threat model. For security-critical work, use current platform guidance and expert review.

