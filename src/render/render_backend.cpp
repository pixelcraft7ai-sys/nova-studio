#include "nova/render/render_backend.hpp"

namespace nova::render {

core::Result<void> NullRenderBackend::initialize() {
  initialized_ = true;
  return {};
}

core::Result<void> RenderBackendRegistry::registerBackend(const std::string& id, Factory factory) {
  if (id.empty() || !factory)
    return core::Error{core::ErrorCode::InvalidArgument, "backend id and factory are required"};
  if (factories_.count(id) != 0)
    return core::Error{core::ErrorCode::InvalidArgument, "backend already registered: " + id};
  factories_.emplace(id, std::move(factory));
  return {};
}

core::Result<std::unique_ptr<IRenderBackend>> RenderBackendRegistry::create(const std::string& id) const {
  auto it = factories_.find(id);
  if (it == factories_.end()) return core::Error{core::ErrorCode::NotFound, "unknown render backend: " + id};
  return it->second();
}

std::vector<std::string> RenderBackendRegistry::ids() const {
  std::vector<std::string> out;
  for (const auto& kv : factories_) out.push_back(kv.first);
  return out;
}

}  // namespace nova::render
