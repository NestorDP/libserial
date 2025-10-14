// @ Copyright 2025 Nestor Neto
//
// Example: list serial ports using Ports class

#include <iostream>
#include "libserial/ports.hpp"

int main() {
    libserial::Ports ports;

    uint16_t num_ports = ports.scanPorts();
    std::cout << "Listed serial ports (if any)." << std::endl;
    std::cout << "Number of devices found: " << num_ports + 1 << std::endl;

    for (uint16_t i = 0; i <= num_ports; ++i) {
        auto name = ports.getName(i);
        auto port_path = ports.getPortPath(i);
        auto bus_path = ports.getBusPath(i);
        std::cout << "  [" << i << "] " << name << " -> " << port_path << " (bus: " << bus_path << ")\n";
    }

    return 0;
}
