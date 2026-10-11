# Security policy

## Reporting a vulnerability
Do **not** open a public issue. The project is private and owned by a single owner: report vulnerabilities directly to the owner (or, once the repo is on GitHub, via a private security advisory). Include the version (`nova-studio --version`), OS, and reproduction steps.
**Never attach your own artwork or client files**; use a synthetic sample.

## Design commitments (enforced by tests where marked)
- Local by default: network and cloud-AI settings default to **off** (tested in `core_test`).
- No secrets in the repository (`secret_scan` test + CI job).
- Logs never contain document content; control characters and over-long messages are
  neutralised (tested). See ADR 0006.
- Dependencies are pinned and listed in `docs/dependencies.md`.

## Supported versions
Pre-release (0.x): only the latest commit on `main`.
