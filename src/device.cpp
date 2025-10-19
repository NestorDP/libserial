//  @ Copyright 2022-2025 Nestor Neto

#include <string>
#include <cstdint>

#include "libserial/device.hpp"

namespace libserial {

Device::Device(const std::string& name,
    const std::string& port_path,
    const std::string& bus_path,
    uint16_t id)
    : name_(name),
    port_path_(port_path),
    bus_path_(bus_path),
    id_(id) {}

std::string Device::getName() const {
    return name_;
}

std::string Device::getPortPath() const {
    return port_path_;
}

std::string Device::getBusPath() const {
    return bus_path_;
}

uint16_t Device::getId() const {
    return id_;
}

void Device::setName(const std::string& name) {
    name_ = name;
}

void Device::setPortPath(const std::string& port_path) {
    port_path_ = port_path;
}

void Device::setBusPath(const std::string& bus_path) {
    bus_path_ = bus_path;
}

void Device::setId(uint16_t id) {
    id_ = id;
}
}  // namespace libserial
