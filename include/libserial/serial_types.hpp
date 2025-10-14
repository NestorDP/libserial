//  @ Copyright 2022-2025 Nestor Neto

/**
 * @file serial_types.hpp
 * @brief Type definitions and enumerations for serial port configuration
 * @author Ntor Neto
 *
 * This file contains type-safe enum classes used for configuring various
 * aspects of serial port communication including parity, stop bits, flow control,
 * canonical mode, terminators, and baud rates.
 */

#ifndef INCLUDE_LIBSERIAL_SERIAL_TYPES_HPP_
#define INCLUDE_LIBSERIAL_SERIAL_TYPES_HPP_

/**
 * @namespace libserial
 * @brief Main namespace for the LibSerial library
 *
 * Contains all classes, functions, and type definitions for serial port
 * communication functionality.
 */
namespace libserial {

struct DeviceStruct {
  std::string name;
  std::string port_path;
  std::string bus_path;
  uint16_t id;
};

/**
 * @enum Parity
 * @brief Enumeration for serial port parity configuration
 *
 * Parity is an error-checking mechanism where an extra bit is added to each
 * byte of data to detect transmission errors. The parity bit makes the total
 * number of 1 bits in the byte either even (even parity) or odd (odd parity).
 *
 * @note This enum currently provides basic enable/disable functionality.
 *       Future versions may include specific parity types (EVEN, ODD, MARK, SPACE).
 */
enum class Parity {
  ENABLE,   ///< Enable parity checking
  DISABLE,  ///< Disable parity checking
};

/**
 * @enum StopBits
 * @brief Enumeration for serial port stop bits configuration
 *
 * Stop bits are used to signal the end of a data frame in asynchronous
 * serial communication. Common configurations include 1 stop bit (standard)
 * or 2 stop bits (for slower or noisy communication).
 *
 * @note This enum currently provides basic enable/disable functionality.
 *       Future versions may include specific stop bit counts (ONE, TWO).
 */
enum class StopBits {
  ENABLE,   ///< Enable stop bits configuration
  DISABLE,  ///< Disable stop bits configuration
};

/**
 * @enum FlowControl
 * @brief Enumeration for serial port flow control configuration
 *
 * Flow control is used to prevent data loss when the receiver cannot keep up
 * with the sender's transmission rate. It provides mechanisms to pause and
 * resume data transmission.
 */
enum class FlowControl {
  HARDWARE,  ///< Hardware flow control (RTS/CTS signals)
  SOFTWARE,  ///< Software flow control (XON/XOFF characters)
};

/**
 * @enum CanonicalMode
 * @brief Enumeration for terminal canonical mode configuration
 *
 * Canonical mode (also known as "cooked mode") controls how input is processed
 * by the terminal driver. In canonical mode, input is line-buffered and
 * processed for special characters (backspace, delete, etc.). In non-canonical
 * mode (raw mode), input is available immediately without line buffering.
 */
enum class CanonicalMode {
  ENABLE,   ///< Enable canonical mode (line-buffered, processed input)
  DISABLE,  ///< Disable canonical mode (raw mode, immediate input)
};

/**
 * @enum Terminator
 * @brief Enumeration for data transmission terminator characters
 *
 * Terminator characters are used to mark the end of data frames or messages
 * in serial communication protocols. These characters help the receiver
 * identify message boundaries in continuous data streams.
 */
enum class Terminator {
  EOT = 4,   ///< End of Transmission (ASCII 4, Ctrl+D)
  CR  = 13,  ///< Carriage Return (ASCII 13, '\r')
};

/**
 * @enum BaudRate
 * @brief Enumeration for serial port baud rate (bits per second) configuration
 *
 * Baud rate defines the speed of data transmission in bits per second (bps).
 * This enum class provides type-safe access to standard baud rates commonly
 * used in serial communication. Higher baud rates allow faster data transfer
 * but may be more susceptible to transmission errors over long distances or
 * in noisy environments.
 *
 * @note The underlying integer values represent the actual baud rate in bps.
 *       These values can be used with system calls that require integer
 *       baud rate parameters through static_cast<int>().
 *
 * @example
 * @code
 * // Type-safe baud rate setting
 * serial.setBaudRate(BaudRate::BAUD_RATE_115200);
 *
 * // Convert to integer if needed for system calls
 * int baud_int = static_cast<int>(BaudRate::BAUD_RATE_9600);
 * @endcode
 */
enum class BaudRate {
  BAUD_RATE_50 = 50,          ///< 50 bps - Very slow, legacy systems
  BAUD_RATE_75 = 75,          ///< 75 bps - Very slow, legacy systems
  BAUD_RATE_110 = 110,        ///< 110 bps - Slow, legacy systems
  BAUD_RATE_134 = 134,        ///< 134 bps - Slow, legacy systems
  BAUD_RATE_150 = 150,        ///< 150 bps - Slow, legacy systems
  BAUD_RATE_200 = 200,        ///< 200 bps - Slow, legacy systems
  BAUD_RATE_300 = 300,        ///< 300 bps - Slow, early modems
  BAUD_RATE_600 = 600,        ///< 600 bps - Slow, early modems
  BAUD_RATE_1200 = 1200,      ///< 1200 bps - Slow, early modems
  BAUD_RATE_1800 = 1800,      ///< 1800 bps - Uncommon rate
  BAUD_RATE_2400 = 2400,      ///< 2400 bps - Low speed applications
  BAUD_RATE_4800 = 4800,      ///< 4800 bps - Low speed applications
  BAUD_RATE_9600 = 9600,      ///< 9600 bps - Common default rate
  BAUD_RATE_19200 = 19200,    ///< 19200 bps - Medium speed applications
  BAUD_RATE_38400 = 38400,    ///< 38400 bps - Medium speed applications
  BAUD_RATE_57600 = 57600,    ///< 57600 bps - High speed applications
  BAUD_RATE_115200 = 115200,  ///< 115200 bps - Very common high speed rate
  BAUD_RATE_230400 = 230400   ///< 230400 bps - Very high speed applications
};

}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_SERIAL_TYPES_HPP_
