# Implementation status

_Last updated: 2026-10-11 (phase 01 documents)_

## Phase 01 — product discovery and governance: **documents delivered; formal acceptance waits for owner approval**

### Delivered (all under `docs/`)
- `product/release-1-scope.md` (in / partial / deferred with reasons), `product/personas.md`, `product/user-journey.md`.
- `product/feature-inventory.md`: 45 features, each tagged "موجودة في الصناعة" (37) or "تميّز مقترح يحتاج تحققًا" (8).
- `product/requirements.md`: 29 functional + 11 non-functional requirements, each with verification method and metric.
- `product/metrics.md`, `product/prioritization-rice.md` (preliminary agent estimates, formula stated), `product/dependency-map.md`.
- `product/privacy-and-rights-framework.md`, `product/claims-policy.md`, `product/naming-status.md`, `product/open-questions.md` (13 questions, each with an owner).
- `product-decisions.md` (13 decisions, with status and source), `risks-and-assumptions.md` (10 risks, 7 unverified assumptions).

### Verified
- `product_docs_check`: every feature classified; deferred features have a reason; every V1/partial feature is covered by a requirement; every requirement has verification + metric; referenced IDs exist; every question has an owner; no forbidden novelty claims. `product_docs_selftest` proves the checker fails on six kinds of bad input.
- Full suite: 14/14 locally (previous 12 + 2 new).

### Not done / honest limits
1. **No user research happened.** Personas, pains and RICE inputs are hypotheses from the blueprint, marked as such. Interviews (Q-06) are needed.
2. **Naming check is a single public web search**, not trademark clearance. Several prior uses of "Nova"/"Nova Studio" exist in nearby fields (see `product/naming-status.md`). Legal review required (Q-01).
3. No patent/prior-art search was done; differentiators stay "تميّز مقترح يحتاج تحققًا".
4. Acceptance item "approval of decisions affecting brand, privacy and business model" is **pending**: DEC-03, DEC-05 (final name), DEC-10, DEC-11 and the Proposed scope (DEC-04).

### Decisions needed from the owner
Approve or change the `Proposed` rows in `docs/product-decisions.md`; answer Q-01…Q-13 as far as possible (at minimum Q-03 business model, Q-04 AI stance, Q-05 first segment).

## Phase 00 — infrastructure and foundation: **COMPLETE — CI green on Ubuntu 24.04, Windows 2022, macOS 14 and the quality job**

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
- `ctest --preset dev`: 12/12 pass (unit tests per module, headless window test, binary smoke test, layering check + its self-test, secret scan + its self-test).
- `ctest --preset asan` (ASan+UBSan): 12/12 pass. Build is warning-free under `-Wall -Wextra -Wpedantic -Wconversion -Werror` etc.
- clang-format applied; cppcheck clean.

### Closed in the second pass
- GitHub Actions pinned to commit SHAs (ADR 0007).
- clang-tidy run on all sources: clean (also added to the CI `quality` job). 
- Licence decided by owner: proprietary, sole owner (ADR 0008); `LICENSE` and `SECURITY.md` updated (no email needed).

### CI verification (GitHub Actions, repo pixelcraft7ai-sys/nova-studio, commit a484ae2)
- `build + test` passed on ubuntu-24.04, windows-2022, macos-14 (Qt 6.5.3, Release, warnings as errors), 12/12 tests each.
- `format + static analysis + sanitizers` passed (clang-format, cppcheck, clang-tidy, ASan+UBSan).
- Problems found and fixed by running CI for real: a malformed cppcheck suppression; Qt Test's variadic macro warning under Apple Clang (broke the macOS build).

### Remaining non-blocking items
1. Qt licence (dynamic LGPL linking) awaits your confirmation; legal review advised before any public release (ADR 0008).
2. Qt downloads in CI are not hash-verified; release age of pinned actions not checked.
3. Window verified offscreen in CI, not by a human on a real display.
4. Hard-coded Arabic strings (translation system: phase 02); atomic write has no fsync (phase 03).

### Clean-clone check (log)
`git clone` into a new directory → `cmake --preset dev` → build → `ctest --preset dev`: 11/11 passed at the time (now 12 tests incl. `app_version_no_display`). First run found that `--version` needed a display; fixed and covered by a test.
