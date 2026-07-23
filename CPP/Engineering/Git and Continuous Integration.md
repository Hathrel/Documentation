# Git and Continuous Integration

Version control records intentional changes; CI proves the repository builds and tests from a clean environment.

## Repository hygiene

Commit source, headers, build scripts, dependency manifests/locks, tests, documentation, and intentional assets. Ignore build directories, object files, executables, editor caches, coverage output, and local secrets.

Make small coherent commits with messages explaining the outcome. Review diffs before committing. Never commit credentials.

## CI pipeline

1. Configure a clean build.
2. Compile with strict warnings.
3. Run tests and show failures.
4. Run formatting/static checks.
5. Run sanitizer builds.
6. Build the supported platform/compiler matrix.
7. Package only after verification.

Pin major tool/dependency versions enough for reproducibility while maintaining an explicit update process. Cache dependencies/builds carefully; a cache miss must not break correctness.

## Release artifacts

Record source revision, compiler/runtime, dependencies, build configuration, and platform. Generate artifacts from tagged, reviewed commits in CI. Sign where your distribution model needs it and publish checksums.

Keep CI commands close to local commands—CMake presets and test commands should behave the same in both environments.

