# 0001 — GUI toolkit: C++17 + Qt 6 (Widgets)
Status: Accepted (provisional, per workplan 00 "initial choice")

**Decision.** Desktop shell in modern C++ with Qt 6 Widgets. Qt is confined to `platform` (Core only) and `app`.
**Why.** Native windows on Windows/macOS/Linux, pen/tablet input, multi-monitor, RTL and Arabic text shaping, mature accessibility bridge.
**Alternatives.** Qt Quick (better for animation, weaker for dense tool UIs); Dear ImGui (no native a11y/RTL); Electron/Tauri (heavy memory for large rasters); Rust + egui/iced (immature RTL/a11y).
**Risks.** LGPL obligations (ADR 0008); Qt 6.4 vs 6.5+ differences — only 6.4.2 verified locally.
**Revisit when.** Phase 02 UX prototype shows RTL/accessibility gaps, or licensing review rejects LGPL use.
