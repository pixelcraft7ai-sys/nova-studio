# Implementation status

_Last updated: 2026-10-11_

## Phase 00 — infrastructure and foundation: **implemented locally; see gaps before calling it complete**

### What exists
- Repo scaffold: LICENSE (provisional), SECURITY, CONTRIBUTING, BUILDING, CHANGELOG, issue template, `.clang-format`, `.clang-tidy`, `.editorconfig`.
- CMake + presets (`dev`, `ci`, `asan`); one static lib per module: `core`, `document`, `render`, `formats`, `platform`, `app`.
- `core`: `Result/Error`, `Logger` (+memory/stream/file sinks, sanitising, UTF-8-safe truncation), schema-validated `Settings`.
- `platform`: `IFileService` + atomic `LocalFileService`, `QtSettingsStore`, `AppPaths`.
- `render`: `IRenderBackend`, `NullRenderBackend`, registry. `formats`: `IFormatHandler` + registry (empty on purpose). `document`: minimal `Document`.
- `app`: `AppServices` DI container, `MainWindow` (menu, side dock, welcome area, status bar), Arabic error presenter, `nova-studio` binary (`--smoke-test`, `--version`, `--config-dir`, `--log-level`).
- Docs: ADRs 0001–0008, `docs/dependencies.md`.
- CI: `.github/workflows/ci.yml` (3 OSes + quality job).

### Verified here (Ubuntu 24.04, GCC 13.3, Qt 6.4.2, CMake 3.28.3)
- `ctest --preset dev`: 11/11 pass (unit tests per module, headless window test, binary smoke test, layering check + its self-test, secret scan + its self-test).
- `ctest --preset asan` (ASan+UBSan): 11/11 pass. Build is warning-free under `-Wall -Wextra -Wpedantic -Wconversion -Werror` etc.
- clang-format applied; cppcheck clean.

### NOT verified / gaps (these block calling the phase accepted)
1. **CI has never run.** Windows and macOS builds are untested; Qt 6.5.3 in CI differs from the 6.4.2 tested here. "Passes CI on declared systems" is therefore unproven.
2. Acceptance item "a new developer builds from a clean environment" was checked only by a fresh clone on this same machine (see log below), not on a clean OS image.
3. GitHub Actions are not pinned to commit SHAs; Qt downloads are not hash-verified (ADR 0007).
4. clang-tidy config exists but clang-tidy was not run.
5. Window opens/closes verified offscreen only, not on a real display.
6. `app` sets RTL and uses hard-coded Arabic strings; a real translation system is phase 02.
7. Atomic write does not fsync (durability is phase 03).

### Decisions needed from you
- **Project licence** (ADR 0008) — blocks any publication.
- **Security contact address** for `SECURITY.md`.
- Confirm Qt as the GUI toolkit given its LGPL/commercial licensing (ADR 0001).
