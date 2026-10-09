# mytools
A collection of tools that I am writing

## CI and releases

GitHub Actions builds and packages Windows x64 and Linux x64 on pushes to
`dev`/`main` and pull requests targeting either branch. CI checks the release
version logic, runs the CTest suite, and verifies ZIP contents.

Merging a pull request **from this repository's `dev` branch into `main`**
automatically builds and publishes a GitHub release. Direct pushes to `main`,
unmerged PRs, and merges from other branches do not publish releases.

- The first release is tagged and titled `mytools-v0.1.0`.
- Each subsequent release increments the highest release tag's patch
  version (for example, `mytools-v0.1.1`).
- To start a new minor/major series, raise the `MYTOOLS_VERSION` default in
  `CMakeLists.txt` on `dev` before merging. CI injects the calculated version;
  it does not commit version bumps back to either branch.
- Releases include Windows ZIP and Linux ZIP, DEB, and RPM downloads. These
  contain `kasm` in `bin` and the license/documentation in `share/mytools`.
  GitHub also provides source archives for the release tag.
- Linux binaries are built on Ubuntu 22.04. RPM packaging does not imply
  compatibility with every RPM-based distribution; dependencies are detected
  automatically and the target must have a compatible glibc.

All platform builds must succeed before publishing. Assets are uploaded to a
draft before the release is made public. Rerunning a published merge reuses its
tag and leaves its release unchanged; rerunning a failed draft upload resumes it.
Keep the `mytools-v*` tag namespace reserved for this workflow.

### Repository setup

Push the CMake sources and `.github` directory to GitHub, and enable GitHub
Actions in repository settings. The publishing job uses the built-in
`GITHUB_TOKEN` with `contents: write`; no personal access token is needed.
Repository/organization policy must allow this permission. Protect `main` by
requiring pull requests and the two `Package` CI checks, and merge release work
through a `dev` -> `main` pull request.

## CMake tests

CTest is enabled through the standard `BUILD_TESTING` option. The debug
configure presets enable it; the release presets disable it. CI explicitly
enables it for release builds on both Windows and Linux, and tests must pass
before packages are published.

In VS Code, select the `windows-debug` or `GCC-debug` configure preset, build,
then use **CMake: Run Tests** (or the Testing panel). The matching test presets
select the correct configuration and show output on failure.

The initial tests live in `tools/kasm/tests`:

- `kasm.loads_input`: loads an existing fixture whose path contains spaces,
  exits successfully, and reports the correct path, byte length, and content.
- `kasm.requires_input`: fails without an argument and prints usage.
- `kasm.missing_file`: fails for a nonexistent file and prints a load error.

Tests check exit status and output together, with timeouts. They are labeled
`kasm`, `cli`, and `input` for filtering. Add future cases alongside the owning
tool or library and register them with `add_test`; guard test subdirectories
with `if(BUILD_TESTING)`. Use `$<TARGET_FILE:...>` rather than hardcoded binary
paths so tests work with single- and multi-configuration generators.

### Quick tutorial: adding a test

For another successful input-loading test, reuse the existing checker:

1. Create `tools/kasm/tests/fixtures/another_input.asm` with some sample text.
   Fixtures are small, version-controlled files that give tests predictable input.
2. Add the following **after** `endforeach()` in
   [tools/kasm/tests/CMakeLists.txt](tools/kasm/tests/CMakeLists.txt):

   ```cmake
   configure_file(
       fixtures/another_input.asm
       "${CMAKE_CURRENT_BINARY_DIR}/another input.asm"
       COPYONLY
   )
   add_test(
       NAME kasm.loads_another_input
       COMMAND ${CMAKE_COMMAND}
           "-DKASM_EXECUTABLE=$<TARGET_FILE:kasm>"
           "-DTEST_CASE=loads_input"
           "-DINPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/another input.asm"
           -P "${CMAKE_CURRENT_SOURCE_DIR}/check_input.cmake"
   )
   set_tests_properties(kasm.loads_another_input PROPERTIES
       LABELS "kasm;cli;input"
       TIMEOUT 10
   )
   ```

   The test name must be unique. `TEST_CASE=loads_input` reuses the checker,
   which reads the new fixture and checks its path, byte length, content, and
   successful exit status. `configure_file(... COPYONLY)` copies the fixture
   into the build directory without changing its contents.
3. Select a debug configure preset, run **CMake: Configure**, then
   **CMake: Build** from the Command Palette (`Ctrl+Shift+P`).
4. Open **View: Show Testing** and run `kasm.loads_another_input`, or use
   **CMake: Run Tests** to run the whole suite. CI picks up registered tests
   automatically; no workflow changes are needed.

For a **new behavior**, add a case name to the `foreach` list and a matching
branch in [check_input.cmake](tools/kasm/tests/check_input.cmake). Set its
arguments and expected output there. Also update the exit-status check:
currently only `loads_input` expects success; all other cases expect failure.
The checker currently expects exact stdout and empty stderr, so adjust those
assertions if the new behavior intentionally changes that contract.

For another tool or a library, create its own `tests/CMakeLists.txt` and include
it from the component's CMake file inside `if(BUILD_TESTING)`. Register either
a CMake checker script or a test executable with `add_test`, and prefix names
with the component name to keep a large suite organized.

### Release-version tests

The workflows install Node.js 24 LTS automatically; Python is not required.
With Node.js installed locally, run
`node --test .github/scripts/release-version.test.cjs` to check automatic
version increments and safe reruns.

### Local packaging

Configure and build using the existing CMake presets. From the configured build
directory, run `cpack -C Release` to create packages using the install rules.
Windows produces ZIP; Linux produces ZIP, DEB, and RPM (requires `dpkg-shlibdeps`
from `dpkg-dev` and `rpmbuild` from `rpm`). Override `MYTOOLS_VERSION` at configure
time to reproduce a particular release. Native Linux packages install under
`/usr`; ZIP archives can be extracted anywhere and include a `usr` directory
containing `bin` and `share/mytools`.
