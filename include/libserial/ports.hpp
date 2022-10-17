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
  std::vector<std::string> * port_list_ ;
  const char *cmd_;  
};

}  // namespace serial

#endif  // LIBSERIAL_PORTS_HPP_
