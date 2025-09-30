//  @ Copyright 2022-2024 Nestor Neto

/**
 * @mainpage Serial Interface Library
 * @section intro_sec Indroduction
 * This is an interface C++ library for serial serial
 * 
 * 
 */
#ifndef LIBSERIAL_SERIAL_HPP_
#define LIBSERIAL_SERIAL_HPP_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <sys/ioctl.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "libserial/serial_exception.hpp"
#include "libserial/serial_types.hpp"

/**
 * @brief Serial Interface Library SERIAL namespace
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
 * @version 1.0.0
 * 
 * Example usage:
 * @code
 * try {
 *     serial::Serial port("/dev/ttyUSB0");
 *     port.setBaudRate(9600);
 *     
 *     auto message = std::make_shared<std::string>("Hello World");
 *     port.write(message);
 *     
 *     std::string response = port.read(100);
 *     std::cout << "Received: " << response << std::endl;
 *     
 * } catch (const SerialException& e) {
 *     std::cerr << "Error: " << e.what() << std::endl;
 * }
 * @endcode
 */
class Serial {
public:
  /**
   * @brief Default constructor
   * 
   * Creates a Serial object without opening any port. 
   * Use open() method to establish connection.
   */
  Serial();

  /**
   * @brief Constructor with automatic port opening
   * 
   * Creates a Serial object and immediately opens the specified port.
   * 
   * @param port The device path (e.g., "/dev/ttyUSB0", "/dev/ttyS0")
   * @throws SerialException if the port cannot be opened
   */
  Serial(std::string port);
  
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
  void open(std::string port);

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
   * @brief Reads data from the serial port
   * 
   * Attempts to read up to max_length bytes from the serial port.
   * This is a blocking operation that will wait for data to arrive.
   * 
   * @param max_length Maximum number of bytes to read
   * @return String containing the read data
   * @throws SerialException if read operation fails
   * 
   * @note The actual number of bytes read may be less than max_length
   */
  std::string read(size_t max_length);

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
  std::string readUntil(char terminator);

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
   * @brief Gets the number of bytes available for reading
   * 
   * Returns the number of bytes currently available in the input buffer
   * without actually reading them. Useful for non-blocking operations.
   * 
   * @return Number of bytes available for reading
   * @throws SerialException if operation fails
   */
  int getAvailableData() const;

  // Future planned methods (not yet implemented):
  
  // void setNumberBits(NumBits num_bits);
  // void setParity(Parity parity);
  // void setStopBits(StopBits stop_bits);
  // void setTimeOut(int time);
  

private:
  /**
   * @brief Retrieves current terminal settings
   * 
   * Internal method to read the current termios2 configuration
   * from the serial port file descriptor.
   * 
   * @throws SerialException if ioctl operation fails
   */
  void GetTermios2();

  /**
   * @brief Applies terminal settings to the port
   * 
   * Internal method to write the termios2 configuration
   * to the serial port file descriptor.
   * 
   * @throws SerialException if ioctl operation fails
   */
  void SetTermios2();

  /**
   * @brief Terminal configuration structure
   * 
   * Holds the current serial port configuration including
   * baud rate, data bits, parity, stop bits, and other settings.
   */
  struct termios2 options_;

  /**
   * @brief File descriptor for the serial port
   * 
   * The low-level file descriptor used for all read/write operations
   * on the serial port. Set to -1 when no port is open.
   */
  int fd_serial_port_;  
};

}  // namespace serial

#endif  // LIBSERIAL_SERIAL_HPP_
