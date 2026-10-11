#pragma once

#include <optional>
#include <string>
#include <utility>
#include <variant>

namespace nova::core {

enum class ErrorCode {
  Unknown,
  InvalidArgument,
  NotFound,
  IoError,
  PermissionDenied,
  Unsupported,
  Corrupt,
  Cancelled,
};

const char* toString(ErrorCode code) noexcept;

// Technical, English, content-free description. User-facing wording is the
// job of the UI layer (see app/ErrorPresenter), never of the core.
struct Error {
  ErrorCode code{ErrorCode::Unknown};
  std::string message;
};

template <typename T>
class Result {
 public:
  Result(T value) : data_(std::move(value)) {}      // NOLINT(google-explicit-constructor)
  Result(Error error) : data_(std::move(error)) {}  // NOLINT(google-explicit-constructor)

  [[nodiscard]] bool ok() const noexcept { return std::holds_alternative<T>(data_); }
  explicit operator bool() const noexcept { return ok(); }

  T& value() & { return std::get<T>(data_); }
  const T& value() const& { return std::get<T>(data_); }
  // cppcheck-suppress returnStdMoveLocal  (data_ is a member, the move is intentional)
  T take() { return std::move(std::get<T>(data_)); }
  [[nodiscard]] const Error& error() const { return std::get<Error>(data_); }

 private:
  std::variant<T, Error> data_;
};

template <>
class Result<void> {
 public:
  Result() = default;
  Result(Error error) : error_(std::move(error)) {}  // NOLINT(google-explicit-constructor)

  [[nodiscard]] bool ok() const noexcept { return !error_.has_value(); }
  explicit operator bool() const noexcept { return ok(); }
  [[nodiscard]] const Error& error() const { return *error_; }

 private:
  std::optional<Error> error_;
};

}  // namespace nova::core
