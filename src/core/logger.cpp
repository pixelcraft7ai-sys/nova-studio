#include "nova/core/logger.hpp"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace nova::core {

const char* toString(LogLevel level) noexcept {
  switch (level) {
    case LogLevel::Trace:
      return "trace";
    case LogLevel::Debug:
      return "debug";
    case LogLevel::Info:
      return "info";
    case LogLevel::Warning:
      return "warning";
    case LogLevel::Error:
      return "error";
    case LogLevel::Critical:
      return "critical";
    case LogLevel::Off:
      return "off";
  }
  return "info";
}

std::optional<LogLevel> parseLogLevel(std::string_view text) noexcept {
  std::string lower;
  for (char c : text) lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
  for (LogLevel l : {LogLevel::Trace, LogLevel::Debug, LogLevel::Info, LogLevel::Warning, LogLevel::Error,
                     LogLevel::Critical, LogLevel::Off}) {
    if (lower == toString(l)) return l;
  }
  return std::nullopt;
}

std::string Logger::sanitize(std::string_view text) {
  std::string out;
  out.reserve(std::min(text.size(), kMaxMessageBytes));
  for (char c : text) {
    const auto u = static_cast<unsigned char>(c);
    out.push_back((u < 0x20 || u == 0x7F) ? ' ' : c);
  }
  if (out.size() <= kMaxMessageBytes) return out;

  std::size_t cut = kMaxMessageBytes;
  // Step back while `cut` points at a UTF-8 continuation byte (10xxxxxx).
  while (cut > 0 && (static_cast<unsigned char>(out[cut]) & 0xC0U) == 0x80U) --cut;
  out.resize(cut);
  out += "...[truncated]";
  return out;
}

void Logger::addSink(std::shared_ptr<ILogSink> sink) {
  if (!sink) return;
  std::lock_guard<std::mutex> lock(mutex_);
  sinks_.push_back(std::move(sink));
}

void Logger::log(LogLevel level, std::string_view category, std::string_view message) {
  if (!isEnabled(level)) return;
  LogRecord record;
  record.level = level;
  record.category = sanitize(category);
  record.message = sanitize(message);
  record.time = std::chrono::system_clock::now();

  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& sink : sinks_) sink->write(record);
}

void MemorySink::write(const LogRecord& record) {
  std::lock_guard<std::mutex> lock(mutex_);
  records_.push_back(record);
}

std::vector<LogRecord> MemorySink::records() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return records_;
}

std::string formatRecord(const LogRecord& record) {
  const std::time_t t = std::chrono::system_clock::to_time_t(record.time);
  std::tm tm{};
#if defined(_WIN32)
  gmtime_s(&tm, &t);
#else
  gmtime_r(&t, &tm);
#endif
  std::ostringstream os;
  os << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ") << ' ' << toString(record.level) << " [" << record.category
     << "] " << record.message;
  return os.str();
}

void StreamSink::write(const LogRecord& record) {
  stream_ << formatRecord(record) << '\n' << std::flush;
}

FileSink::FileSink(const std::string& path) : file_(path, std::ios::app) {}

void FileSink::write(const LogRecord& record) {
  if (file_.is_open()) file_ << formatRecord(record) << '\n' << std::flush;
}

}  // namespace nova::core
