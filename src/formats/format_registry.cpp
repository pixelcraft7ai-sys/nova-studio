#include <algorithm>
#include <cctype>

#include "nova/formats/format_handler.hpp"

namespace nova::formats {

namespace {
std::string lower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return s;
}
}  // namespace

core::Result<void> FormatRegistry::registerHandler(std::shared_ptr<IFormatHandler> handler) {
  if (!handler) return core::Error{core::ErrorCode::InvalidArgument, "null format handler"};
  for (const auto& h : handlers_) {
    if (h->id() == handler->id())
      return core::Error{core::ErrorCode::InvalidArgument, "format already registered: " + handler->id()};
  }
  handlers_.push_back(std::move(handler));
  return {};
}

std::shared_ptr<IFormatHandler> FormatRegistry::findByExtension(const std::string& extension) const {
  const std::string wanted = lower(extension);
  for (const auto& h : handlers_) {
    for (const auto& ext : h->extensions())
      if (lower(ext) == wanted) return h;
  }
  return nullptr;
}

std::vector<std::string> FormatRegistry::ids() const {
  std::vector<std::string> out;
  out.reserve(handlers_.size());
  for (const auto& h : handlers_) out.push_back(h->id());
  return out;
}

}  // namespace nova::formats
