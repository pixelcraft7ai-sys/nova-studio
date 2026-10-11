#include "nova/platform/qt_settings_store.hpp"

#include <QDir>
#include <QSettings>
#include <QString>

namespace nova::platform {

using core::Error;
using core::ErrorCode;

namespace {
QString k(std::string_view key) {
  return QString::fromUtf8(key.data(), static_cast<qsizetype>(key.size()));
}
}  // namespace

QtSettingsStore::QtSettingsStore(const std::string& directory) {
  QDir().mkpath(QString::fromStdString(directory));
  const QString file = QDir(QString::fromStdString(directory)).filePath(QStringLiteral("settings.ini"));
  settings_ = std::make_unique<QSettings>(file, QSettings::IniFormat);
}

QtSettingsStore::~QtSettingsStore() = default;

std::optional<std::string> QtSettingsStore::read(std::string_view key) const {
  const QString name = k(key);
  if (!settings_->contains(name)) return std::nullopt;
  return settings_->value(name).toString().toStdString();
}

core::Result<void> QtSettingsStore::write(std::string_view key, std::string_view value) {
  settings_->setValue(k(key), QString::fromUtf8(value.data(), static_cast<qsizetype>(value.size())));
  settings_->sync();
  if (settings_->status() != QSettings::NoError)
    return Error{ErrorCode::IoError, "cannot write settings file"};
  return {};
}

core::Result<void> QtSettingsStore::remove(std::string_view key) {
  settings_->remove(k(key));
  settings_->sync();
  if (settings_->status() != QSettings::NoError)
    return Error{ErrorCode::IoError, "cannot write settings file"};
  return {};
}

}  // namespace nova::platform
