#pragma once

#include <memory>
#include <optional>
#include <string>

#include "error_presenter.hpp"
#include "nova/core/logger.hpp"
#include "nova/core/settings.hpp"
#include "nova/formats/format_handler.hpp"
#include "nova/platform/file_service.hpp"
#include "nova/render/render_backend.hpp"

namespace nova::app {

// Dependency-injection container. UI classes receive this instead of reaching
// for global singletons (workplan 00, task 4).
struct AppServices {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<core::Settings> settings;
  std::shared_ptr<platform::IFileService> files;
  std::shared_ptr<render::RenderBackendRegistry> renderBackends;
  std::shared_ptr<formats::FormatRegistry> formats;
  std::shared_ptr<IErrorPresenter> errors;

  [[nodiscard]] bool isComplete() const {
    return logger && settings && files && renderBackends && formats && errors;
  }
};

struct AppOptions {
  std::optional<std::string> configDir;  // overrides the per-user config location
  std::optional<core::LogLevel> logLevel;
  bool headless{false};  // true -> errors are logged, never shown in a modal dialog
};

AppServices createDefaultServices(const AppOptions& options);

}  // namespace nova::app
