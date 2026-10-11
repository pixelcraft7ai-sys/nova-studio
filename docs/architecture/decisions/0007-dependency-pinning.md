# 0007 — Dependency pinning and provenance
Status: Accepted (partially applied)
Dependencies are recorded in `docs/dependencies.md` with version, source, licence, verification. CI installs Qt at a pinned version (6.5.3).
GitHub Actions are pinned to **commit SHAs** (resolved with `git ls-remote` on 2026-10-11): checkout v7.0.1, upload-artifact v7.0.2, install-qt-action v4.4.1.
**Remaining gap:** Qt packages downloaded by `install-qt-action` are not hash-verified by us; I did not check release ages of the pinned actions (some may be < 14 days old). Review before first release.
