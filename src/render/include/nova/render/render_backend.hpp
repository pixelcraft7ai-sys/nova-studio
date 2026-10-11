#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "nova/core/result.hpp"

namespace nova::render {

struct RenderCapabilities {
  bool gpuAccelerated{false};
  int maxTextureSize{0};
};

// Abstraction over the drawing engine (ADR 0003). The first real backend is
// chosen in phase 04 after benchmarks; nothing above this interface may depend
// on Vulkan/Metal/D3D12/OpenGL types.
class IRenderBackend {
 public:
  virtual ~IRenderBackend() = default;
  [[nodiscard]] virtual std::string name() const = 0;
  [[nodiscard]] virtual RenderCapabilities capabilities() const = 0;
  virtual core::Result<void> initialize() = 0;
  virtual void shutdown() = 0;
};

// CPU placeholder so the app, tests and CI work with no GPU. It performs no
// drawing; it exists to prove the seam, not to be a renderer.
class NullRenderBackend final : public IRenderBackend {
 public:
  [[nodiscard]] std::string name() const override { return "null"; }
  [[nodiscard]] RenderCapabilities capabilities() const override { return {}; }
  core::Result<void> initialize() override;
  void shutdown() override { initialized_ = false; }
  [[nodiscard]] bool isInitialized() const { return initialized_; }

 private:
  bool initialized_{false};
};

class RenderBackendRegistry {
 public:
  using Factory = std::function<std::unique_ptr<IRenderBackend>()>;

  core::Result<void> registerBackend(const std::string& id, Factory factory);
  [[nodiscard]] core::Result<std::unique_ptr<IRenderBackend>> create(const std::string& id) const;
  [[nodiscard]] std::vector<std::string> ids() const;

 private:
  std::map<std::string, Factory> factories_;
};

}  // namespace nova::render
