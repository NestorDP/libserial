//  @ Copyright 2022 Nestor Neto

/**
 * @mainpage Serial Interface Library
 * @section intro_sec Indroduction
 * This is an interface C++ library for serial serial
 * 
 * 
 */
#ifndef INCLUDE_LIBSERIAL_PORTS_HPP_
#define INCLUDE_LIBSERIAL_PORTS_HPP_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

namespace libserial {

struct DeviceStruct {
  std::string name;
  std::string port;
  std::string path;
  int id;
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
  std::vector<DeviceStruct> device_list_;
};

}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_PORTS_HPP_
