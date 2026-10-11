# 0003 — Render backend abstraction; engine NOT yet chosen
Status: Accepted (abstraction only)
**Decision.** `render::IRenderBackend` + registry. Only a `null` backend exists. No Vulkan/Metal/D3D12/OpenGL type may appear outside a backend.
**Why.** The blueprint requires benchmarks on low/mid/high hardware before choosing. This ADR deliberately does **not** claim the final GPU engine is decided.
**Revisit when.** Phase 04 benchmark results are available.
