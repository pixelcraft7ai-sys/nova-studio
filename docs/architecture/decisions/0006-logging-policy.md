# 0006 — Logging and message policy
Status: Accepted
- Levels: trace, debug, info, warning, error, critical, off (default info).
- Log events/technical identifiers only. **Never** log pixels, typed text, file contents, or full user paths.
- Messages are stripped of control characters (anti log-injection) and capped at 1024 bytes without splitting UTF-8 (tested, incl. Arabic).
- Core errors are English and technical; the UI shows plain Arabic text per error code and puts the technical text in expandable details.
- Sinks: stderr and `<log dir>/nova.log` (no rotation yet — operations phase 13).
