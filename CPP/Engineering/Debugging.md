# Debugging

Debugging is evidence gathering, not guessing.

## Reproducible process

1. Capture exact input, configuration, environment, build, and error.
2. Minimize the failing case.
3. Form one falsifiable hypothesis.
4. Inspect or instrument the boundary that distinguishes outcomes.
5. Fix the root cause and add a regression test.

## Debugger skills

- Set breakpoints and conditional breakpoints.
- Step over, into, and out.
- Inspect locals, expressions, memory, and call stack.
- Add watchpoints/data breakpoints to stop when memory changes.
- Inspect all threads and lock ownership for hangs.
- Read exception and signal locations.

Build with debug symbols (`-g`, `/Zi`) and retain the exact binary for crash dumps. Optimized builds are harder to step through but often necessary to reproduce production behavior.

## Logging

Log stable identifiers, decisions, durations, errors, and relevant state—not secrets or huge raw payloads. Include `std::source_location` where useful. Structured logs are easier to query than prose. Avoid changing timing excessively while investigating concurrency.

## Crash categories

- Segmentation/access violation: suspect invalid pointer, bounds, lifetime, race.
- Abort/assertion: read the condition and stack; find the earlier state corruption.
- Uncaught exception: inspect type, message, throw site, and ownership cleanup.
- Hang: break all threads; find waits, deadlocks, infinite loops, blocked I/O.
- Wrong result: check inputs and invariants at component boundaries; binary-search execution/state.

Run sanitizers early; they often report the first invalid action rather than the later crash. See [[Engineering/Warnings, Sanitizers, and Static Analysis]].

