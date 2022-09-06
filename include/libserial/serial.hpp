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
#include <string>

#define BUFFER_LEN 512
#define TCP 1
#define UDP 0

/**
 * @brief Ethernet Interface Library SERIAL namespace
 * @author Nestor Pereira Neto
 * 
 */

namespace serial {

class Serial {
 public:
  /**
   * @brief Construct a new Ethernet Interface object
   * 
   */
  Serial();

  /**
   * @brief Destroy the Ethernet Interface object
   * 
   */
  ~Serial();

    /**
   * @brief Destroy the Ethernet Interface object
   * 
   */
  void open_port(std::string port);

  /**
   * @brief Send string over serial port
   * 
   * @param const char *msg
   */
  void send_msg(std::string msg);

  /**
   * @brief Receive string over serial port
   * 
   * @param
   */
  void receive_msg(std::string msg);

 private:
  /** File descriptor for the port */
  int fd_serial_port_;

  /** Length of buffer */
  int kLengthBuffer_ = 200;

  /**  */
  int n_;
};

}  // namespace serial

#endif  // LIBSERIAL_SERIAL_HPP_
