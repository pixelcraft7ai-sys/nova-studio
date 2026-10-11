# 0004 — Local by default; no network code in the foundation
Status: Accepted
**Decision.** No networking, uploads or content analytics exist in phase 00. Settings `privacy.network_enabled`, `privacy.cloud_ai`, `privacy.crash_reports` default to `false` and are covered by tests. Any future network feature must read these settings and show visible consent.
**Revisit when.** Phases 08/10 introduce AI routing and sync.
