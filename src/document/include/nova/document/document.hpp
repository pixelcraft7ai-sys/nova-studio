#pragma once

#include <cstdint>
#include <string>

namespace nova::document {

// Phase 00 contract only: identity, name, and change tracking. The node graph,
// commands, undo/redo and the .nova container arrive in phase 03.
// This module must stay free of Qt and of any GUI type.
class Document {
 public:
  explicit Document(std::string name);

  [[nodiscard]] const std::string& id() const noexcept { return id_; }
  [[nodiscard]] const std::string& name() const noexcept { return name_; }
  void setName(std::string name);

  // Increases on every mutation. Used by renderers/caches to invalidate.
  [[nodiscard]] std::uint64_t revision() const noexcept { return revision_; }
  [[nodiscard]] bool isModified() const noexcept { return revision_ != savedRevision_; }
  void markSaved() noexcept { savedRevision_ = revision_; }

 private:
  std::string id_;
  std::string name_;
  std::uint64_t revision_{0};
  std::uint64_t savedRevision_{0};
};

}  // namespace nova::document
