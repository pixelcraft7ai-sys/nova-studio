#include "nova/core/result.hpp"

namespace nova::core {

const char* toString(ErrorCode code) noexcept {
  switch (code) {
    case ErrorCode::Unknown:
      return "Unknown";
    case ErrorCode::InvalidArgument:
      return "InvalidArgument";
    case ErrorCode::NotFound:
      return "NotFound";
    case ErrorCode::IoError:
      return "IoError";
    case ErrorCode::PermissionDenied:
      return "PermissionDenied";
    case ErrorCode::Unsupported:
      return "Unsupported";
    case ErrorCode::Corrupt:
      return "Corrupt";
    case ErrorCode::Cancelled:
      return "Cancelled";
  }
  return "Unknown";
}

}  // namespace nova::core
