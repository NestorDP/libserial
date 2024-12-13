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

namespace serial {

class Serial {
public:
  Serial();

  Serial(std::string port);
  
  ~Serial();
  
  void open(std::string port);

  void close();

  void write(std::string& data);

  void write(const void* data, size_t length);

  std::string read(size_t max_length);

  int availableData(int fd);

  void setBaudRate(int baud_rate);

  // void setNumberBits(NumBits num_bits);

  // void setParity(Parity parity);

  // void setStopBits(StopBits stop_bits);

  // void setTimeOut(int time);
  

private:
  void GetTermios2();

  void SetTermios2();

  struct termios2 options_;

  int fd_serial_port_;  
};

}  // namespace serial

#endif  // LIBSERIAL_SERIAL_HPP_
