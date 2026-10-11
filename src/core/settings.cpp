#include "nova/core/settings.hpp"

#include <charconv>

namespace nova::core {

std::optional<std::string> MemorySettingsStore::read(std::string_view key) const {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = values_.find(key);
  if (it == values_.end()) return std::nullopt;
  return it->second;
}

Result<void> MemorySettingsStore::write(std::string_view key, std::string_view value) {
  std::lock_guard<std::mutex> lock(mutex_);
  values_[std::string(key)] = std::string(value);
  return {};
}

Result<void> MemorySettingsStore::remove(std::string_view key) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = values_.find(key);
  if (it != values_.end()) values_.erase(it);
  return {};
}

const std::vector<SettingDefinition>& settingDefinitions() {
  static const std::vector<SettingDefinition> defs = {
      {"privacy.network_enabled", SettingType::Bool, "false", "Allow any network access. Off = fully local."},
      {"privacy.cloud_ai", SettingType::Bool, "false",
       "Allow sending content to a cloud AI provider (requires explicit consent)."},
      {"privacy.crash_reports", SettingType::Bool, "false", "Send crash reports (opt-in)."},
      {"ui.language", SettingType::String, "ar", "Interface language code."},
      {"logging.level", SettingType::String, "info", "Minimum log level."},
  };
  return defs;
}

Settings::Settings(std::shared_ptr<ISettingsStore> store) : store_(std::move(store)) {}

Result<const SettingDefinition*> Settings::lookup(std::string_view key, SettingType type) const {
  for (const auto& def : settingDefinitions()) {
    if (def.key == key) {
      if (def.type != type)
        return Error{ErrorCode::InvalidArgument, "setting has a different type: " + std::string(key)};
      return &def;
    }
  }
  return Error{ErrorCode::NotFound, "unknown setting: " + std::string(key)};
}

std::string Settings::rawOrDefault(const SettingDefinition& def) const {
  if (store_) {
    if (auto v = store_->read(def.key)) return *v;
  }
  return std::string(def.defaultValue);
}

Result<bool> Settings::getBool(std::string_view key) const {
  auto def = lookup(key, SettingType::Bool);
  if (!def) return def.error();
  const std::string raw = rawOrDefault(*def.value());
  if (raw == "true") return true;
  if (raw == "false") return false;
  return Error{ErrorCode::Corrupt, "stored value is not a bool: " + std::string(key)};
}

Result<std::int64_t> Settings::getInt(std::string_view key) const {
  auto def = lookup(key, SettingType::Int);
  if (!def) return def.error();
  const std::string raw = rawOrDefault(*def.value());
  std::int64_t out = 0;
  const auto [ptr, ec] = std::from_chars(raw.data(), raw.data() + raw.size(), out);
  if (ec != std::errc{} || ptr != raw.data() + raw.size())
    return Error{ErrorCode::Corrupt, "stored value is not an int: " + std::string(key)};
  return out;
}

Result<std::string> Settings::getString(std::string_view key) const {
  auto def = lookup(key, SettingType::String);
  if (!def) return def.error();
  return rawOrDefault(*def.value());
}

Result<void> Settings::setBool(std::string_view key, bool value) {
  auto def = lookup(key, SettingType::Bool);
  if (!def) return def.error();
  return store_->write(key, value ? "true" : "false");
}

Result<void> Settings::setInt(std::string_view key, std::int64_t value) {
  auto def = lookup(key, SettingType::Int);
  if (!def) return def.error();
  return store_->write(key, std::to_string(value));
}

Result<void> Settings::setString(std::string_view key, std::string_view value) {
  auto def = lookup(key, SettingType::String);
  if (!def) return def.error();
  return store_->write(key, value);
}

Result<void> Settings::reset(std::string_view key) {
  for (const auto& def : settingDefinitions())
    if (def.key == key) return store_->remove(key);
  return Error{ErrorCode::NotFound, "unknown setting: " + std::string(key)};
}

}  // namespace nova::core
