#pragma once

#include <QString>
#include <vector>

#include "nova/core/result.hpp"

namespace nova::app {

// Plain-Arabic explanation for an error code. The technical English message
// from the core is shown only as expandable detail, never as the headline.
QString userMessageFor(core::ErrorCode code);

// UI-agnostic seam so tests and headless runs never open modal dialogs.
class IErrorPresenter {
 public:
  virtual ~IErrorPresenter() = default;
  virtual void present(const core::Error& error, const QString& context) = 0;
};

class MessageBoxErrorPresenter final : public IErrorPresenter {
 public:
  void present(const core::Error& error, const QString& context) override;
};

class RecordingErrorPresenter final : public IErrorPresenter {
 public:
  struct Entry {
    core::Error error;
    QString context;
  };
  void present(const core::Error& error, const QString& context) override {
    entries.push_back({error, context});
  }
  std::vector<Entry> entries;
};

}  // namespace nova::app
