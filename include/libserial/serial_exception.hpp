//  @ Copyright 2022-2025 Nestor Neto

#ifndef INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_
#define INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_

#include <exception>
#include <string>
#include <utility>

namespace libserial {
class SerialException : public std::exception {
public:
explicit SerialException(std::string message) : message_(std::move(message)) {
}

const char* what() const noexcept override {
  return message_.c_str();
}

private:
std::string message_;  // NOLINT(runtime/string)
};
}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_
