#include "app_services.hpp"

#include <iostream>

#include "nova/platform/app_paths.hpp"
#include "nova/platform/qt_settings_store.hpp"

namespace nova::app {

AppServices createDefaultServices(const AppOptions& options) {
  AppServices s;
  const auto paths = platform::resolveAppPaths(options.configDir);

  s.files = std::make_shared<platform::LocalFileService>();
  s.files->createDirectories(paths.logDir);

  s.settings = std::make_shared<core::Settings>(std::make_shared<platform::QtSettingsStore>(paths.configDir));

  s.logger = std::make_shared<core::Logger>(core::LogLevel::Info);
  s.logger->addSink(std::make_shared<core::StreamSink>(std::cerr));
  auto fileSink = std::make_shared<core::FileSink>(paths.logDir + "/nova.log");
  if (fileSink->isOpen()) s.logger->addSink(fileSink);

  if (options.logLevel) {
    s.logger->setLevel(*options.logLevel);
  } else if (auto configured = s.settings->getString("logging.level")) {
    if (auto parsed = core::parseLogLevel(configured.value())) s.logger->setLevel(*parsed);
  }

  s.renderBackends = std::make_shared<render::RenderBackendRegistry>();
  s.renderBackends->registerBackend("null", [] { return std::make_unique<render::NullRenderBackend>(); });
  s.formats = std::make_shared<formats::FormatRegistry>();  // empty until phase 09

  if (options.headless) {
    s.errors = std::make_shared<RecordingErrorPresenter>();
  } else {
    s.errors = std::make_shared<MessageBoxErrorPresenter>();
  }
  return s;
}

}  // namespace nova::app
