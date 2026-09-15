[[17 - Namespaces, Headers, and Modules#Headers|Headers]] do not have to be in the same folder as a `.cpp` file. Include paths configured in the compiler or build system determine where headers are found.

A `.cpp` file does not inherently need a matching header. A header is needed when declarations or definitions must be shared with other translation units. The `.cpp` that supplies an interface's definitions should include its matching header first so the compiler verifies that they agree.

A header should be self-contained and directly include what its own declarations require. It should not rely on another file having included those dependencies first.
