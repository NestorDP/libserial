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
        auto name = ports.findName(i);
        auto port_path = ports.findPortPath(i);
        auto bus_path = ports.findBusPath(i);
        std::cout << "  [" << i << "] " << name.value_or("unknown") << " -> " << port_path.value_or("unknown") << " (bus: " << bus_path.value_or("unknown") << ")\n";
    }

    return 0;
}
