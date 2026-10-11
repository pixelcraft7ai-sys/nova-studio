# Contributing

1. Read `BUILDING.md` and make a clean build with the `dev` preset.
2. Respect the layering (ADR 0005). `ctest` runs `layering_check`; it fails if `core`,
   `document`, `render` or `formats` include Qt, or if a lower layer includes a higher one.
3. Format with `clang-format -i` (config in `.clang-format`); CI rejects unformatted code.
4. Every feature ships with an automated test, or a documented manual test procedure.
5. Do not claim support for a file format, platform or standard before its tests pass.
6. Never commit secrets, API keys or user files. Never log user content.
7. Record significant choices as ADRs in `docs/architecture/decisions/`.
8. Update `docs/implementation-status.md` with what was done, tested, and what remains.

## Bug reports
Use the template in `.github/ISSUE_TEMPLATE/bug_report.md`. Attach the log file only after
reading it: logs are content-free by policy, but check anyway.
