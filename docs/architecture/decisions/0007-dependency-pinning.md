# 0007 — Dependency pinning and provenance
Status: Accepted (partially applied)
Dependencies are recorded in `docs/dependencies.md` with version, source, licence, verification. CI installs Qt at a pinned version. **Gap:** GitHub Actions are pinned by tag, not commit SHA, and Qt packages are not hash-verified by us; tracked as a follow-up in the status file.
