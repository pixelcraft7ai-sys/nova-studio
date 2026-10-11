#include "nova/platform/app_paths.hpp"

#include <QDir>
#include <QStandardPaths>

namespace nova::platform {

AppPaths resolveAppPaths(const std::optional<std::string>& overrideDir) {
  AppPaths paths;
  if (overrideDir && !overrideDir->empty()) {
    const QDir base(QString::fromStdString(*overrideDir));
    paths.configDir = base.absolutePath().toStdString();
    paths.logDir = base.filePath(QStringLiteral("logs")).toStdString();
    return paths;
  }
  const QString config = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  const QString data = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  paths.configDir = config.toStdString();
  paths.logDir = QDir(data).filePath(QStringLiteral("logs")).toStdString();
  return paths;
}

}  // namespace nova::platform
