//  @ Copyright 2022-2025 Nestor Neto

#ifndef INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_
#define INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_

#include <exception>
#include <string>
#include <utility>

namespace libserial {
/**
 * @class SerialException
 * @brief Exception class for serial port errors
 *
 * The SerialException class is used to represent errors that occur
 * during serial port operations. It inherits from std::exception and
 * provides a descriptive error message.
 *
 * @author Nestor Pereira Neto
 */
class SerialException : public std::exception {
public:
explicit SerialException(std::string message)
  : message_(std::move(message)) {
}
const char* what() const noexcept override {
  return message_.c_str();
}
private:
std::string message_;
};  // class SerialException

/**
 * @class PortNotFoundException
 * @brief Exception class for port not found errors
 *
 * The PortNotFoundException class is derived from SerialException
 * and is used to indicate that a specified serial port could not be found.
 */
class PortNotFoundException : public SerialException {
public:
explicit PortNotFoundException(std::string message)
  : SerialException(std::move(message)) {
}
};  // class PortNotFoundException

/**
 * @class PermissionDeniedException
 * @brief Exception class for permission denied errors
 *
 * The PermissionDeniedException class is derived from SerialException
 * and is used to indicate that permission to access a specified serial port was denied.
 */
class PermissionDeniedException : public SerialException {
public:
explicit PermissionDeniedException(std::string message)
  : SerialException(std::move(message)) {
}
};  // class PermissionDeniedException

/**
 * @class TimeoutException
 * @brief Exception class for timeout errors
 *
 * The TimeoutException class is derived from SerialException
 * and is used to indicate that a serial port operation has timed out.
 */
class TimeoutException : public SerialException {
public:
explicit TimeoutException(std::string message)
  : SerialException(std::move(message)) {
}
};  // class TimeoutException

/**
 * @class IOException
 * @brief Exception class for I/O errors
 *
 * The IOException class is derived from SerialException
 * and is used to indicate that an I/O error has occurred during
 * serial port operations.
 */
class IOException : public SerialException {
public:
explicit IOException(std::string message)
  : SerialException(std::move(message)) {
}
};  // class IOException

}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_SERIAL_EXCEPTION_HPP_
