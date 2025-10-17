//  @ Copyright 2022-2025 Nestor Neto

#ifndef INCLUDE_LIBSERIAL_SERIAL_HPP_
#define INCLUDE_LIBSERIAL_SERIAL_HPP_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <sys/ioctl.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "libserial/serial_exception.hpp"
#include "libserial/serial_types.hpp"

/**
 * @brief Serial Interface Library libserial namespace
 * @author Nestor Pereira Neto
 *
 */

namespace libserial {

/**
 * @brief A C++ wrapper class for serial port communication
 *
 * The Serial class provides a high-level interface for serial port operations
 * on Linux systems. It encapsulates low-level POSIX system calls and provides
 * exception-based error handling for serial communication.
 *
 * @author Nestor Pereira Neto
 * @version 0.0.0
 */
class Serial {
public:
/**
 * @brief Default constructor
 *
 * Creates a Serial object without opening any port.
 * Use open() method to establish connection.
 * Initializes with default baud rate of 9600.
 */
Serial() = default;
Serial(const Serial&) = delete;
Serial& operator=(const Serial&) = delete;

/**
 * @brief Constructor with automatic port opening
 *
 * Creates a Serial object and immediately opens the specified port.
 *
 * @param port The device path (e.g., "/dev/ttyUSB0", "/dev/ttyS0")
 * @throws SerialException if the port cannot be opened
 */
explicit Serial(const std::string& port);

/**
 * @brief Destructor
 *
 * Automatically closes the serial port if it's open, ensuring
 * proper resource cleanup.
 */
~Serial();

/**
 * @brief Opens a serial port for communication
 *
 * @param port The device path to open (e.g., "/dev/ttyUSB0")
 * @throws SerialException if the port doesn't exist or cannot be accessed
 * @throws PermissionDeniedException if insufficient permissions
 */
void open(const std::string& port);

/**
 * @brief Closes the currently open serial port
 *
 * @throws SerialException if an error occurs during closing
 */
void close();

/**
 * @brief Writes data to the serial port
 *
 * Sends the provided string data to the serial port. A carriage return
 * character ('\\r') is automatically appended to the data.
 *
 * @param data Shared pointer to the string data to write
 * @throws SerialException if write operation fails
 * @throws std::invalid_argument if data pointer is null
 *
 * @note The original string is not modified; a copy is made with the
 *       terminator appended.
 */
void write(std::shared_ptr<std::string> data);

/**
 * @brief Reads data from serial port into a shared pointer buffer
 *
 * Reads up to max_length bytes from the serial port and stores them
 * in the provided shared string buffer. This version provides better
 * memory management and avoids unnecessary string copies.
 *
 * @param buffer Shared pointer to string where data will be stored
 * @param max_length Maximum number of bytes to read
 * @return Number of bytes actually read
 * @throws SerialException if read operation fails
 * @throws SerialException if buffer is null
 *
 * @note The buffer will be resized to contain exactly the read data
 */
size_t read(std::shared_ptr<std::string> buffer, size_t max_length);

/**
 * @brief Reads data until a specific terminator character is found
 *
 * Continues reading byte by byte until the specified terminator
 * character is encountered. The terminator is included in the result.
 *
 * @param terminator The character to stop reading at
 * @return String containing all read data including the terminator
 * @throws SerialException if read operation fails
 *
 * @warning This method reads one byte at a time and may be slower
 *          for large amounts of data
 */
size_t readUntil(std::shared_ptr<std::string> buffer, char terminator);

/**
 * @brief Flushes the input buffer
 *
 * Discards any data that has been received but not yet read.
 * Useful for clearing stale data before starting fresh communication.
 *
 * @throws SerialException if flush operation fails
 */
void flushInputBuffer();

/**
 * @brief Sets the baud rate for serial communication
 *
 * Configures the speed of serial communication. Both input and output
 * speeds are set to the same value.
 *
 * @param baud_rate The desired baud rate (e.g., 9600, 115200)
 * @throws SerialException if baud rate cannot be set
 *
 * @note The port must be opened before calling this method
 */
void setBaudRate(int baud_rate);

/**
 * @brief Set the baud rate using BaudRate enum
 *
 * This overloaded version accepts a BaudRate enum value, providing
 * type safety and preventing invalid baud rate values.
 *
 * @param baud_rate The baud rate from BaudRate enum
 * @throws SerialException if baud rate cannot be set
 *
 * @note The port must be opened before calling this method
 */
void setBaudRate(BaudRate baud_rate);

/**
 * @brief Gets the current baud rate
 *
 * @return The current baud rate
 * @throws SerialException if unable to retrieve baud rate
 */
int getBaudRate() const;

/**
 * @brief Gets the number of bytes available for reading
 *
 * Returns the number of bytes currently available in the input buffer
 * without actually reading them. Useful for non-blocking operations.
 *
 * @return Number of bytes available for reading
 * @throws SerialException if operation fails
 */
int getAvailableData() const;

/**
 * @brief Sets the read timeout in milliseconds
 *
 * Configures the maximum time to wait for read operations before
 * timing out. A value of 0 means no timeout (blocking).
 *
 * @param timeout Timeout in milliseconds
 */
void setReadTimeout(unsigned int timeout);

/**
 * @brief Sets the write timeout in milliseconds
 *
 * Configures the maximum time to wait for write operations before
 * timing out. A value of 0 means no timeout (blocking).
 *
 * @param timeout Timeout in milliseconds
 */
void setWriteTimeout(unsigned int timeout);

// Future planned methods (not yet implemented):
// void setNumberBits(NumBits num_bits);
// void setParity(Parity parity);
// void setStopBits(StopBits stop_bits);


private:
/**
 * @brief Retrieves current terminal settings
 *
 * Internal method to read the current termios2 configuration
 * from the serial port file descriptor.
 *
 * @throws SerialException if ioctl operation fails
 */
void getTermios2() const;

/**
 * @brief Applies terminal settings to the port
 *
 * Internal method to write the termios2 configuration
 * to the serial port file descriptor.
 *
 * @throws SerialException if ioctl operation fails
 */
void setTermios2();

/**
 * @brief Terminal configuration structure
 *
 * Holds the current serial port configuration including
 * baud rate, data bits, parity, stop bits, and other settings.
 */
mutable struct termios2 options_;

/**
 * @brief File descriptor for the serial port
 *
 * The low-level file descriptor used for all read/write operations
 * on the serial port. Set to -1 when no port is open.
 */
int fd_serial_port_{-1};

/**
 * @brief Read timeout in milliseconds
 *
 * Specifies the maximum time to wait for read operations
 * before timing out. Default is 1000ms.
 */
unsigned int read_timeout_{1000};    ///< Read timeout in milliseconds (default 1000ms)

/**
 * @brief Write timeout in milliseconds
 *
 * Specifies the maximum time to wait for write operations
 * before timing out. Default is 1000ms.
 */
unsigned int write_timeout_{1000};    ///< Write timeout in milliseconds (default 1000ms)
};

}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_SERIAL_HPP_
