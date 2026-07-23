# Dependencies and Package Managers

Use a dependency when its maintained, tested functionality exceeds the lifetime cost of adopting it.

## Evaluation checklist

- License compatible with your project.
- Maintenance activity, release history, security response.
- Supported compilers/platforms and C++ standard.
- API stability, documentation, tests, thread/lifetime model.
- Transitive dependency and binary-size impact.
- Reproducible version pinning and offline/CI availability.

## Common managers

- **vcpkg:** manifest mode with `vcpkg.json`; integrates with CMake toolchains.
- **Conan:** recipes/profiles and generated CMake integration.
- **System packages:** useful for platform deployments, versions vary.
- **CMake FetchContent:** convenient source dependency mechanism; pin immutable revisions.

Prefer config-mode CMake packages exposing imported targets:

```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries(app PRIVATE fmt::fmt)
```

Do not globally add include/link directories. Lock versions, review updates, and cache artifacts appropriately. Do not silently fetch the internet during every ordinary build if reproducibility or restricted environments matter.

## ABI concerns

C++ binary interfaces can vary with compiler, standard library, build flags, debug mode, and dependency version. Avoid passing standard-library types across unstable plugin/compiler boundaries unless toolchains are controlled. A C ABI with opaque handles or a serialized protocol is often a more stable boundary.

Commit manifests and lockfiles according to the manager’s recommended reproducibility model, not downloaded build output.

