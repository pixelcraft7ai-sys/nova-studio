#include "nova/document/document.hpp"

#include <cstdio>
#include <random>

namespace nova::document {

namespace {
std::string generateId() {
  std::random_device rd;
  std::string out;
  out.reserve(32);
  for (int i = 0; i < 4; ++i) {
    char buf[9];
    std::snprintf(buf, sizeof(buf), "%08x", static_cast<unsigned>(rd()));
    out += buf;
  }
  return out;
}
}  // namespace

Document::Document(std::string name) : id_(generateId()), name_(std::move(name)) {}

void Document::setName(std::string name) {
  if (name == name_) return;
  name_ = std::move(name);
  ++revision_;
}

}  // namespace nova::document
