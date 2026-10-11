#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "nova/core/result.hpp"

namespace nova::core {

// Raw string storage. Implemented in memory here (tests) and over QSettings in
// the platform module. Knows nothing about the schema.
class ISettingsStore {
 public:
  virtual ~ISettingsStore() = default;
  [[nodiscard]] virtual std::optional<std::string> read(std::string_view key) const = 0;
  virtual Result<void> write(std::string_view key, std::string_view value) = 0;
  virtual Result<void> remove(std::string_view key) = 0;
};

class MemorySettingsStore final : public ISettingsStore {
 public:
  [[nodiscard]] std::optional<std::string> read(std::string_view key) const override;
  Result<void> write(std::string_view key, std::string_view value) override;
  Result<void> remove(std::string_view key) override;

 private:
  mutable std::mutex mutex_;
  std::map<std::string, std::string, std::less<>> values_;
};

enum class SettingType { Bool, Int, String };

struct SettingDefinition {
  std::string_view key;
  SettingType type;
  std::string_view defaultValue;  // textual form: "true"/"false", decimal, or text
  std::string_view description;
};

// Every setting the app understands. Privacy-relevant defaults are the
// restrictive ones: nothing leaves the machine unless the user turns it on.
const std::vector<SettingDefinition>& settingDefinitions();

// Typed, schema-validated view over a store. Unknown keys and wrongly typed
// values are rejected instead of silently stored.
class Settings {
 public:
  explicit Settings(std::shared_ptr<ISettingsStore> store);

  [[nodiscard]] Result<bool> getBool(std::string_view key) const;
  [[nodiscard]] Result<std::int64_t> getInt(std::string_view key) const;
  [[nodiscard]] Result<std::string> getString(std::string_view key) const;

  Result<void> setBool(std::string_view key, bool value);
  Result<void> setInt(std::string_view key, std::int64_t value);
  Result<void> setString(std::string_view key, std::string_view value);
  Result<void> reset(std::string_view key);

 private:
  [[nodiscard]] Result<const SettingDefinition*> lookup(std::string_view key, SettingType type) const;
  [[nodiscard]] std::string rawOrDefault(const SettingDefinition& def) const;

  std::shared_ptr<ISettingsStore> store_;
};

}  // namespace nova::core
