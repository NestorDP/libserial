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
#include <algorithm>
#include <optional>

#include "libserial/serial_exception.hpp"
#include "libserial/serial_types.hpp"

/**
 * @brief Ethernet Interface Library SERIAL namespace
 * @author Nestor Pereira Neto
 *
 */

namespace libserial {



class Ports {
public:
/**
 * @brief Constructor of the Ports class
 *
 */
Ports() = default;

/**
 * @brief Destroyer of the Ports class
 *
 */
~Ports() = default;

/**
 * @brief Receive string over serial port
 *
 * @param N/D
 * @return std::string
 */
uint16_t scanPorts();


void getDeviceList(std::vector<DeviceStruct> & list) const;

std::optional<std::string> findPortPath(uint16_t id) const;

std::optional<std::string> findBusPath(uint16_t id) const;

std::optional<std::string> findName(uint16_t id) const;

private:
std::vector<DeviceStruct> device_list_;
};
}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_PORTS_HPP_
