//  @ Copyright 2022 Nestor Neto

/**
 * @mainpage Serial Interface Library
 * @section intro_sec Indroduction
 * This is an interface C++ library for serial serial
 * 
 * 
 */
#ifndef LIBSERIAL_PORTS_HPP_
#define LIBSERIAL_PORTS_HPP_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Ethernet Interface Library SERIAL namespace
 * @author Nestor Pereira Neto
 * 
 */

namespace serial {

struct DeviceStruct {
  std::string name;
  std::string port;
  std::string path;
} device;


class Ports {
 public:
  /**
   * @brief Constructor of the Ports class
   * 
   */
  Ports();

  /**
   * @brief Destroyer of the Ports class
   * 
   */
  ~Ports();

  /**
   * @brief Receive string over serial port
   * 
   * @param N/D
   * @return std::string 
   */
  void list_ports();


 private:
  std::vector<DeviceStruct> device_list ;
  int num_devices_ = 1;
};

}  // namespace serial

#endif  // LIBSERIAL_PORTS_HPP_
