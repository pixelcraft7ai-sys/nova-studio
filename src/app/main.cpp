#include <QApplication>
#include <QCommandLineParser>
#include <iostream>
#include <string>

#include "main_window.hpp"
#include "nova/core/version.hpp"

int main(int argc, char* argv[]) {
  // --version must work on machines without a display (CI, SSH), so answer it
  // before any GUI object exists.
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--version" || arg == "-v") {
      std::cout << nova::core::kAppName << ' ' << nova::core::kVersion << '\n';
      return 0;
    }
  }

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName(QStringLiteral("NOVA"));
  QCoreApplication::setApplicationName(QStringLiteral("NOVA Studio"));
  QCoreApplication::setApplicationVersion(QString::fromUtf8(nova::core::kVersion));

  QCommandLineParser parser;
  parser.setApplicationDescription(QStringLiteral("NOVA Studio"));
  parser.addHelpOption();
  parser.addVersionOption();
  const QCommandLineOption smoke(QStringLiteral("smoke-test"),
                                 QStringLiteral("Create the main window, then close it and exit 0."));
  const QCommandLineOption configDir(QStringLiteral("config-dir"),
                                     QStringLiteral("Store settings and logs under <dir>."),
                                     QStringLiteral("dir"));
  const QCommandLineOption logLevel(QStringLiteral("log-level"),
                                    QStringLiteral("trace|debug|info|warning|error|critical|off"),
                                    QStringLiteral("level"));
  parser.addOptions({smoke, configDir, logLevel});
  parser.process(app);

  nova::app::AppOptions options;
  options.headless = parser.isSet(smoke);
  if (parser.isSet(configDir)) options.configDir = parser.value(configDir).toStdString();
  if (parser.isSet(logLevel)) {
    const auto level = nova::core::parseLogLevel(parser.value(logLevel).toStdString());
    if (!level) {
      std::cerr << "invalid --log-level value\n";
      return 2;
    }
    options.logLevel = level;
  }

  nova::app::AppServices services = nova::app::createDefaultServices(options);
  const auto language = services.settings->getString("ui.language");
  if (!language || language.value() == "ar") app.setLayoutDirection(Qt::RightToLeft);

  nova::app::MainWindow window(services);
  window.show();

  if (parser.isSet(smoke)) {
    QCoreApplication::processEvents();
    const bool visible = window.isVisible();
    window.close();
    services.logger->info("app", visible ? "smoke test passed" : "smoke test failed: window not visible");
    return visible ? 0 : 1;
  }
  return app.exec();
}
