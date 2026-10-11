#pragma once

#include <optional>
#include <string>

namespace nova::platform {

struct AppPaths {
  std::string configDir;
  std::string logDir;
};

// Resolves per-user directories. QCoreApplication organisation/application
// names must be set first. `overrideDir` (used by tests and --config-dir)
// makes everything live under that single directory.
AppPaths resolveAppPaths(const std::optional<std::string>& overrideDir = std::nullopt);

}  // namespace nova::platform
