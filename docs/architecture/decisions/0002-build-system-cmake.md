# 0002 — Build system: CMake + presets
Status: Accepted
**Decision.** CMake ≥ 3.21 with `CMakePresets.json` (`dev`, `ci`, `asan`); one static library per module; warnings as errors in dev/CI; optional ASan/UBSan and clang-tidy.
**Alternatives.** Meson (smaller ecosystem for Qt+CI), Bazel (heavy), qmake (deprecated in Qt 6).
**Revisit when.** Package management needs outgrow CMake + a lock table (consider vcpkg/Conan at phase 03+).
