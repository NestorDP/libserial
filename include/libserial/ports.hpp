//  @ Copyright 2022-2025 Nestor Neto

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

#include "libserial/device.hpp"
#include "libserial/serial_exception.hpp"
#include "libserial/serial_types.hpp"

namespace libserial {

/**
 * @brief A class to manage and list available serial ports on the system
 *
 * The Ports class provides functionality to scan for available serial ports,
 * retrieve their details, and find specific ports by unique identifiers.
 * It leverages the udev system on Linux to discover connected serial devices.
 *
 * @author Nestor Pereira Neto
 */
class Ports {
public:
/**
 * @brief Constructor of the Ports class
 *
 */
Ports() = default;

/**
 * @brief Testable constructor allowing a custom system path
 *
 * Primarily intended for testing to inject a non-existent or
 * non-readable directory to validate error handling paths.
 */
explicit Ports(const char* sys_path) : sys_path_(sys_path) {}

/**
 * @brief Destroyer of the Ports class
 *
 */
~Ports() = default;

/**
 * @brief Scans the system for available serial ports
 *
 * @return uint16_t The number of serial ports found
 * @throws SerialException if no ports are found
 */
uint16_t scanPorts();

/**
 * @brief Retrieves the list of detected serial devices
 *
 * @param devices A reference to a vector that will be populated with
 *             Device entries for each detected device
 * @throws SerialException if device list cannot be retrieved
 */
void getDevices(std::vector<Device> & devices) const;

/**
 * @brief Finds the port path for a device with the specified ID
 *
 * @param id The unique identifier of the device to search for
 * @return std::optional<std::string> The port path if found, or std::nullopt if not found
 * @throws SerialException if search operation fails
 */
std::optional<std::string> findPortPath(uint16_t id) const;

/**
 * @brief Finds the bus path for a device with the specified ID
 *
 * @param id The unique identifier of the device to search for
 * @return std::optional<std::string> The bus path if found, or std::nullopt if not found
 * @throws SerialException if search operation fails
 */
std::optional<std::string> findBusPath(uint16_t id) const;

/**
 * @brief Finds the name for a device with the specified ID
 *
 * @param id The unique identifier of the device to search for
 * @return std::optional<std::string> The name if found, or std::nullopt if not found
 * @throws SerialException if search operation fails
 */
std::optional<std::string> findName(uint16_t id) const;

private:
/**
 * @brief System path where udev creates symlinks for serial devices by ID
 */
static constexpr const char* kSysSerialByIdPath = "/dev/serial/by-id/";

/**
 * @brief Configurable path used by scanPorts; defaults to kSysSerialByIdPath
 */
const char* sys_path_ { kSysSerialByIdPath };

/**
 * @brief Internal list of detected serial devices
 */
std::vector<Device> devices_;
};
}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_PORTS_HPP_
