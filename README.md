# mytools
A collection of tools that I am writing

## CI and releases

GitHub Actions builds and packages Windows x64 and Linux x64 on pushes to
`dev`/`main` and pull requests targeting either branch. CI checks the release
version logic, runs the executable, and verifies ZIP contents. There is not yet
a CTest suite.

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
