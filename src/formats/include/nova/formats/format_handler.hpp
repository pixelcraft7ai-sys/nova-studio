#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "nova/core/result.hpp"
#include "nova/document/document.hpp"

namespace nova::formats {

using Bytes = std::vector<std::uint8_t>;

// One handler per file format. Phase 09 adds real handlers; per project rules
// no format may be advertised as supported before its tests pass.
class IFormatHandler {
 public:
  virtual ~IFormatHandler() = default;
  [[nodiscard]] virtual std::string id() const = 0;
  [[nodiscard]] virtual std::string displayName() const = 0;
  [[nodiscard]] virtual std::vector<std::string> extensions() const = 0;  // lower case, no dot
  [[nodiscard]] virtual bool canImport() const = 0;
  [[nodiscard]] virtual bool canExport() const = 0;
  virtual core::Result<std::unique_ptr<document::Document>> import(const Bytes& data) = 0;
  virtual core::Result<Bytes> exportDocument(const document::Document& doc) = 0;
};

class FormatRegistry {
 public:
  core::Result<void> registerHandler(std::shared_ptr<IFormatHandler> handler);
  [[nodiscard]] std::shared_ptr<IFormatHandler> findByExtension(const std::string& extension) const;
  [[nodiscard]] std::vector<std::string> ids() const;

 private:
  std::vector<std::shared_ptr<IFormatHandler>> handlers_;
};

}  // namespace nova::formats
