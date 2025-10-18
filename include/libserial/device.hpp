//  @ Copyright 2022-2025 Nestor Neto

#ifndef INCLUDE_LIBSERIAL_DEVICE_HPP_
#define INCLUDE_LIBSERIAL_DEVICE_HPP_

#include <string>
#include <cstdint>

namespace libserial {

class Device {
public:
Device() = default;
~Device() = default;

Device(const std::string& name,
        const std::string& port_path,
        const std::string& bus_path,
        uint16_t id)
    : name_(name),
        port_path_(port_path),
        bus_path_(bus_path),
        id_(id) {}

std::string getName() const {
    return name_;
}

std::string getPortPath() const {
    return port_path_;
}

std::string getBusPath() const {
    return bus_path_;
}

uint16_t getId() const {
    return id_;
}

void setName(const std::string& name) {
    name_ = name;
}

void setPortPath(const std::string& port_path) {
    port_path_ = port_path;
}

void setBusPath(const std::string& bus_path) {
    bus_path_ = bus_path;
}

void setId(uint16_t id) {
    id_ = id;
}

private:
std::string name_;
std::string port_path_;
std::string bus_path_;
uint16_t id_{0};
};

}  // namespace libserial
#endif  // INCLUDE_LIBSERIAL_DEVICE_HPP_
