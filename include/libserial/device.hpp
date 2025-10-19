//  @ Copyright 2022-2025 Nestor Neto

#ifndef INCLUDE_LIBSERIAL_DEVICE_HPP_
#define INCLUDE_LIBSERIAL_DEVICE_HPP_

#include <string>
#include <cstdint>

namespace libserial {

/**
 * @brief A class representing a serial device
 *
 * The Device class encapsulates the properties of a serial device,
 * including its name, port path, bus path, and unique identifier.
 *
 * @author Nestor Pereira Neto
 */
class Device {
public:
/**
 * @brief Default constructor of the Device class
 *
 */
Device() = default;

/**
 * @brief Default destructor of the Device class
 *
 */
~Device() = default;

/**
 * @brief Parameterized constructor of the Device class
 *
 * @param name The name of the device
 * @param port_path The port path of the device
 * @param bus_path The bus path of the device
 * @param id The unique identifier of the device
 */
Device(const std::string& name,
    const std::string& port_path,
    const std::string& bus_path,
    uint16_t id);

/**
 * @brief Retrieves the name of the device
 *
 * @return std::string The name of the device
 */
std::string getName() const;

/**
 * @brief Retrieves the port path of the device
 *
 * @return std::string The port path of the device
 */
std::string getPortPath() const;

/**
 * @brief Retrieves the bus path of the device
 *
 * @return std::string The bus path of the device
 */
std::string getBusPath() const;

/**
 * @brief Retrieves the unique identifier of the device
 *
 * @return uint16_t The unique identifier of the device
 */
uint16_t getId() const;

/**
 * @brief Sets the name of the device
 *
 * @param name The name to set
 */
void setName(const std::string& name);

/**
 * @brief Sets the port path of the device
 *
 * @param port_path The port path to set
 */
void setPortPath(const std::string& port_path);

/**
 * @brief Sets the bus path of the device
 *
 * @param bus_path The bus path to set
 */
void setBusPath(const std::string& bus_path);

/**
 * @brief Sets the unique identifier of the device
 *
 * @param id The unique identifier to set
 */
void setId(uint16_t id);

private:
/**
 * @brief The name of the device
 */
std::string name_{"unknown"};

/**
 * @brief The port path of the device
 */
std::string port_path_{"unknown"};

/**
 * @brief The bus path of the device
 */
std::string bus_path_{"unknown"};

/**
 * @brief The unique identifier of the device
 */
uint16_t id_{0};
};
}  // namespace libserial

#endif  // INCLUDE_LIBSERIAL_DEVICE_HPP_
