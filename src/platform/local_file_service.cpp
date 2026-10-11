#include <filesystem>
#include <fstream>
#include <random>

#include "nova/platform/file_service.hpp"

namespace nova::platform {

namespace fs = std::filesystem;
using core::Error;
using core::ErrorCode;

bool LocalFileService::exists(const std::string& path) const {
  std::error_code ec;
  return fs::exists(fs::u8path(path), ec);
}

core::Result<Bytes> LocalFileService::readAll(const std::string& path) const {
  std::error_code ec;
  const fs::path p = fs::u8path(path);
  if (!fs::exists(p, ec)) return Error{ErrorCode::NotFound, "file not found"};
  if (!fs::is_regular_file(p, ec)) return Error{ErrorCode::InvalidArgument, "not a regular file"};

  std::ifstream in(p, std::ios::binary);
  if (!in) return Error{ErrorCode::PermissionDenied, "cannot open file for reading"};
  Bytes data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  if (in.bad()) return Error{ErrorCode::IoError, "read failed"};
  return data;
}

core::Result<void> LocalFileService::writeAtomic(const std::string& path, const Bytes& data) {
  const fs::path target = fs::u8path(path);
  std::random_device rd;
  fs::path tmp = target;
  tmp += ".tmp-" + std::to_string(rd());

  {
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) return Error{ErrorCode::IoError, "cannot create temporary file"};
    out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    out.flush();
    if (!out) {
      std::error_code ignore;
      fs::remove(tmp, ignore);
      return Error{ErrorCode::IoError, "write failed"};
    }
  }
  std::error_code ec;
  fs::rename(tmp, target, ec);
  if (ec) {
    std::error_code ignore;
    fs::remove(tmp, ignore);
    return Error{ErrorCode::IoError, "cannot replace target file"};
  }
  return {};
}

core::Result<void> LocalFileService::createDirectories(const std::string& path) {
  std::error_code ec;
  fs::create_directories(fs::u8path(path), ec);
  if (ec) return Error{ErrorCode::IoError, "cannot create directory"};
  return {};
}

}  // namespace nova::platform
