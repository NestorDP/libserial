//  @ Copyright 2022 Nestor Neto

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
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>

/**
 * @brief Ethernet Interface Library SERIAL namespace
 * @author Nestor Pereira Neto
 * 
 */

namespace serial {

class Serial {
 private:
  /** Struct t */
  struct termios tty_;

  /** File descriptor for the port */
  int fd_serial_port_;

  /** Length of buffer */
  int kLengthBuffer_ = 200;

  /**  */
  int n_;

 public:
  /**
   * @brief Constructor of the serial class
   * 
   */
  Serial();

  /**
   * @brief Destroyer of the serial class
   * 
   */
  ~Serial();

  /**
   * @brief Method to open the serial communication
   * 
   */
  void OpenPort(std::string port);

  /**
   * @brief Send string over serial port
   * 
   * @param std::shared_ptr<std::string> msg_ptr
   */
  void SendMsg(std::shared_ptr<std::string> msgPtr);

  /**
   * @brief Receive string over serial port
   * 
   * @param N/D
   * @return void 
   */
  void ReceiveMsg(std::string* msgPtr);

  /**
   * @brief Configure the number of bits per byte
   * 
   * @param int numBitsits
   * @return void
   */
  void SetNumberBits(int mumBits);
};

}  // namespace serial

#endif  // LIBSERIAL_SERIAL_HPP_
