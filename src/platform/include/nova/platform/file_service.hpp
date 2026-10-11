#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "nova/core/result.hpp"

namespace nova::platform {

using Bytes = std::vector<std::uint8_t>;

// All file access goes through this interface so it can be faked in tests and
// replaced per platform (sandboxed stores, network drives, ...).
class IFileService {
 public:
  virtual ~IFileService() = default;
  [[nodiscard]] virtual bool exists(const std::string& path) const = 0;
  [[nodiscard]] virtual core::Result<Bytes> readAll(const std::string& path) const = 0;
  // Writes to a temporary sibling file and renames over `path`, so a failure
  // never leaves a half-written target. (Durability/fsync policy: phase 03.)
  virtual core::Result<void> writeAtomic(const std::string& path, const Bytes& data) = 0;
  virtual core::Result<void> createDirectories(const std::string& path) = 0;
};

class LocalFileService final : public IFileService {
 public:
  [[nodiscard]] bool exists(const std::string& path) const override;
  [[nodiscard]] core::Result<Bytes> readAll(const std::string& path) const override;
  core::Result<void> writeAtomic(const std::string& path, const Bytes& data) override;
  core::Result<void> createDirectories(const std::string& path) override;
};

}  // namespace nova::platform
