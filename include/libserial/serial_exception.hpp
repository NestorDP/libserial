#ifndef SERIAL_EXCEPTION_HPP
#define SERIAL_EXCEPTION_HPP

#include <exception>
#include <string>

namespace serial {
class SerialException : public std::exception {
 public:
  explicit SerialException(const std::string& message) : message_(message) {}

  virtual const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};
}  // namespace serial

#endif // SERIAL_EXCEPTION_HPP