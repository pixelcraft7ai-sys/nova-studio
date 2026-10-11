#pragma once

#include <memory>
#include <string>

#include "nova/core/settings.hpp"

class QSettings;

namespace nova::platform {

// Persists settings in an INI file inside `directory`.
class QtSettingsStore final : public core::ISettingsStore {
 public:
  explicit QtSettingsStore(const std::string& directory);
  ~QtSettingsStore() override;

  [[nodiscard]] std::optional<std::string> read(std::string_view key) const override;
  core::Result<void> write(std::string_view key, std::string_view value) override;
  core::Result<void> remove(std::string_view key) override;

 private:
  std::unique_ptr<QSettings> settings_;
};

}  // namespace nova::platform
