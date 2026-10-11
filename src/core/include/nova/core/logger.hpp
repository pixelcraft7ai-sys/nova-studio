#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "nova/core/result.hpp"

namespace nova::core {

// Logging policy (see docs/architecture/decisions/0006-logging-policy.md):
//  * Log events and technical identifiers, NEVER user content (image pixels,
//    text typed into documents, file contents, full personal paths).
//  * Messages are sanitised (control characters removed) and length-capped.
enum class LogLevel { Trace, Debug, Info, Warning, Error, Critical, Off };

const char* toString(LogLevel level) noexcept;
std::optional<LogLevel> parseLogLevel(std::string_view text) noexcept;

struct LogRecord {
  LogLevel level{LogLevel::Info};
  std::string category;
  std::string message;
  std::chrono::system_clock::time_point time{};
};

class ILogSink {
 public:
  virtual ~ILogSink() = default;
  virtual void write(const LogRecord& record) = 0;
};

class Logger {
 public:
  static constexpr std::size_t kMaxMessageBytes = 1024;

  explicit Logger(LogLevel minimum = LogLevel::Info) : level_(minimum) {}

  void setLevel(LogLevel level) noexcept { level_.store(level); }
  [[nodiscard]] LogLevel level() const noexcept { return level_.load(); }
  [[nodiscard]] bool isEnabled(LogLevel level) const noexcept {
    return level >= this->level() && this->level() != LogLevel::Off;
  }

  void addSink(std::shared_ptr<ILogSink> sink);
  void log(LogLevel level, std::string_view category, std::string_view message);

  void debug(std::string_view category, std::string_view message) { log(LogLevel::Debug, category, message); }
  void info(std::string_view category, std::string_view message) { log(LogLevel::Info, category, message); }
  void warning(std::string_view category, std::string_view message) {
    log(LogLevel::Warning, category, message);
  }
  void error(std::string_view category, std::string_view message) { log(LogLevel::Error, category, message); }

  // Removes control characters (prevents log injection), and truncates to
  // kMaxMessageBytes without splitting a UTF-8 sequence.
  static std::string sanitize(std::string_view text);

 private:
  std::atomic<LogLevel> level_;
  std::mutex mutex_;
  std::vector<std::shared_ptr<ILogSink>> sinks_;
};

class MemorySink final : public ILogSink {
 public:
  void write(const LogRecord& record) override;
  [[nodiscard]] std::vector<LogRecord> records() const;

 private:
  mutable std::mutex mutex_;
  std::vector<LogRecord> records_;
};

class StreamSink final : public ILogSink {
 public:
  explicit StreamSink(std::ostream& stream) : stream_(stream) {}
  void write(const LogRecord& record) override;

 private:
  std::ostream& stream_;
};

class FileSink final : public ILogSink {
 public:
  explicit FileSink(const std::string& path);
  [[nodiscard]] bool isOpen() const { return file_.is_open(); }
  void write(const LogRecord& record) override;

 private:
  std::ofstream file_;
};

std::string formatRecord(const LogRecord& record);

}  // namespace nova::core
