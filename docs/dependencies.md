# Dependencies

Policy: every dependency is listed here with version, source, licence, and how it was
verified. Add nothing without a row. No dependency is fetched at build time from a URL
that is not listed.

| Dependency | Required | Version exercised | Source | Licence | Verification / risk |
|---|---|---|---|---|---|
| Qt 6 (Core, Widgets, Test) | yes | **6.4.2** (Ubuntu 24.04 `qt6-base-dev`, tested locally); CI pins **6.5.3** via `jurplel/install-qt-action` (not yet run — see status) | Distro packages / Qt online installer | LGPLv3 / GPLv3 / commercial | Linked dynamically; keep dynamic linking and ship licence texts. Final licensing decision: ADR 0008 (open). |
| CMake | build | ≥ 3.21 (3.28.3 tested) | cmake.org / distro | BSD-3 | — |
| Ninja | build | any | distro | Apache-2.0 | optional |
| clang-format | dev/CI | 18 | distro | Apache-2.0 w/ LLVM exception | style only |
| cppcheck | dev/CI | distro | distro | GPLv3 | analysis tool, not shipped |
| GitHub Actions: `actions/checkout` v7.0.1, `actions/upload-artifact` v7.0.2, `jurplel/install-qt-action` v4.4.1 | CI | pinned to commit SHAs (see ci.yml) | github.com | MIT / various | SHAs resolved via `git ls-remote`; release age not checked. |

No third-party code is vendored. Phase 03+ will add compression/ICC/etc.; each needs a row and an ADR.
