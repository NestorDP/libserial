Examples
========

This section provides practical examples of using libserial.

Basic Serial Communication
---------------------------

Simple Echo Server
~~~~~~~~~~~~~~~~~~

Create a simple echo server that reads data and sends it back:

.. code-block:: cpp

    #include <iostream>
    #include <memory>
    #include <thread>
    #include <chrono>
    #include <string>

    #include "libserial/serial.hpp"
    #include "libserial/serial_exception.hpp"

    int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <serial_port>" << std::endl;
        std::cerr << "Example: " << argv[0] << " /tmp/ttyV0" << std::endl;
        std::cerr << std::endl;
        std::cerr << "To create virtual ports with socat:" << std::endl;
        std::cerr << "socat -d -d pty,link=/tmp/ttyV0,raw,echo=0 pty,link=/tmp/ttyV1,raw,echo=0" << std::endl;
        return 1;
    }

    std::string port = argv[1];

    try {
        // Create and configure serial port
        libserial::Serial serial;

        std::cout << "Opening serial port: " << port << std::endl;
        serial.open(port);

        std::cout << "Setting baud rate to 115200..." << std::endl;
        serial.setBaudRate(libserial::BaudRate::BAUD_RATE_115200);

        // Verify configuration
        int current_baud = serial.getBaudRate();
        std::cout << "Current baud rate: " << current_baud << std::endl;

        // Send a message
        auto message = std::make_shared<std::string>("Hello from libserial!");
        std::cout << "Sending message: '" << *message << "'" << std::endl;
        serial.write(message);

        // Wait a bit for potential response
        std::cout << "Waiting for response..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Check for available data
        int available = serial.getAvailableData();
        std::cout << "Available data: " << available << " bytes" << std::endl;

        if (available > 0) {
        auto buffer = std::make_shared<std::string>();
        size_t bytes_read = serial.read(buffer);
        std::cout << "Received (" << bytes_read << " bytes): '" << *buffer << "'" << std::endl;
        } else {
        std::cout << "No response received." << std::endl;
        std::cout << "You can send data to the other end of the virtual port." << std::endl;
        }

        // Interactive mode
        std::cout << std::endl;
        std::cout << "Interactive mode - type messages (Ctrl+C to exit):" << std::endl;
        std::string input;

        while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        // Send user input
        auto user_message = std::make_shared<std::string>(input);
        serial.write(user_message);
        std::cout << "Sent: '" << input << "'" << std::endl;

        // Check for response
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        available = serial.getAvailableData();

        if (available > 0) {
            auto buffer = std::make_shared<std::string>();
            size_t bytes_read = serial.read(buffer);
            std::cout << "Received (" << bytes_read << " bytes): '" << *buffer << "'" << std::endl;
        }
        }
    } catch (const libserial::SerialException& e) {
        std::cerr << "Serial error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
    }


.. Data Logger
.. ~~~~~~~~~~~

.. Log incoming serial data to a file:

.. .. code-block:: cpp

..    #include <libserial/serial.hpp>
..    #include <iostream>
..    #include <fstream>
..    #include <chrono>
..    #include <iomanip>

..    int main() {
..        try {
..            libserial::Serial serial("/dev/ttyUSB0");
..            serial.setBaudRate(libserial::BaudRate::BAUD_115200);
..            serial.setTimeout(std::chrono::seconds(1));
..            serial.open();

..            std::ofstream logFile("serial_log.txt");
           
..            std::cout << "Data logger started. Logging to serial_log.txt" << std::endl;

..            while (true) {
..                try {
..                    std::string data = serial.read(256);
                   
..                    // Get current timestamp
..                    auto now = std::chrono::system_clock::now();
..                    auto time_t = std::chrono::system_clock::to_time_t(now);
                   
..                    // Log with timestamp
..                    logFile << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
..                           << " - " << data << std::endl;
..                    logFile.flush();
                   
..                    std::cout << "Logged: " << data << std::endl;
                   
..                } catch (const libserial::TimeoutException& e) {
..                    // Timeout is expected, continue
..                    continue;
..                }
..            }

..        } catch (const libserial::SerialException& e) {
..            std::cerr << "Error: " << e.what() << std::endl;
..            return 1;
..        }

..        return 0;
..    }

.. Device Communication
.. --------------------

.. AT Command Interface
.. ~~~~~~~~~~~~~~~~~~~~

.. Communicate with AT command-based devices (modems, GPS modules, etc.):

.. .. code-block:: cpp

..    #include <libserial/serial.hpp>
..    #include <iostream>
..    #include <string>
..    #include <chrono>

..    class ATDevice {
..    private:
..        libserial::Serial serial_;
       
..    public:
..        ATDevice(const std::string& port) : serial_(port) {
..            serial_.setBaudRate(libserial::BaudRate::BAUD_115200);
..            serial_.setTimeout(std::chrono::seconds(5));
..        }
       
..        void connect() {
..            serial_.open();
..            std::cout << "Connected to device" << std::endl;
..        }
       
..        std::string sendCommand(const std::string& command) {
..            // Send command with CR+LF
..            serial_.write(command + "\r\n");
           
..            // Read response
..            std::string response;
..            try {
..                response = serial_.read(1024);
..            } catch (const libserial::TimeoutException& e) {
..                throw std::runtime_error("Command timeout: " + command);
..            }
           
..            return response;
..        }
       
..        bool testConnection() {
..            try {
..                std::string response = sendCommand("AT");
..                return response.find("OK") != std::string::npos;
..            } catch (const std::exception& e) {
..                return false;
..            }
..        }
..    };

..    int main() {
..        try {
..            ATDevice device("/dev/ttyUSB0");
..            device.connect();
           
..            if (device.testConnection()) {
..                std::cout << "Device is responding" << std::endl;
               
..                // Get device information
..                std::string info = device.sendCommand("ATI");
..                std::cout << "Device info: " << info << std::endl;
               
..            } else {
..                std::cout << "Device is not responding" << std::endl;
..            }

..        } catch (const std::exception& e) {
..            std::cerr << "Error: " << e.what() << std::endl;
..            return 1;
..        }

..        return 0;
..    }

.. Sensor Data Collection
.. ~~~~~~~~~~~~~~~~~~~~~~

.. Collect data from a serial sensor:

.. .. code-block:: cpp

..    #include <libserial/serial.hpp>
..    #include <iostream>
..    #include <sstream>
..    #include <vector>

..    struct SensorReading {
..        double temperature;
..        double humidity;
..        double pressure;
..    };

..    class SensorReader {
..    private:
..        libserial::Serial serial_;
       
..    public:
..        SensorReader(const std::string& port) : serial_(port) {
..            serial_.setBaudRate(libserial::BaudRate::BAUD_9600);
..            serial_.setTimeout(std::chrono::seconds(2));
..        }
       
..        void connect() {
..            serial_.open();
..        }
       
..        SensorReading readSensor() {
..            // Request sensor data
..            serial_.write("READ\n");
           
..            // Read response (assuming CSV format: temp,humidity,pressure)
..            std::string response = serial_.read(100);
           
..            // Parse CSV data
..            std::istringstream ss(response);
..            std::string token;
..            std::vector<double> values;
           
..            while (std::getline(ss, token, ',')) {
..                values.push_back(std::stod(token));
..            }
           
..            if (values.size() != 3) {
..                throw std::runtime_error("Invalid sensor data format");
..            }
           
..            return {values[0], values[1], values[2]};
..        }
..    };

..    int main() {
..        try {
..            SensorReader sensor("/dev/ttyUSB0");
..            sensor.connect();
           
..            for (int i = 0; i < 10; ++i) {
..                SensorReading reading = sensor.readSensor();
               
..                std::cout << "Reading " << (i + 1) << ": "
..                         << "Temp=" << reading.temperature << "°C, "
..                         << "Humidity=" << reading.humidity << "%, "
..                         << "Pressure=" << reading.pressure << "hPa"
..                         << std::endl;
               
..                std::this_thread::sleep_for(std::chrono::seconds(1));
..            }

..        } catch (const std::exception& e) {
..            std::cerr << "Error: " << e.what() << std::endl;
..            return 1;
..        }

..        return 0;
..    }

Port Discovery
--------------

Finding Available Ports
~~~~~~~~~~~~~~~~~~~~~~~~

Discover and list available serial ports:

.. code-block:: cpp

    #include <iostream>
    #include <vector>

    #include "libserial/ports.hpp"
    #include "libserial/device.hpp"

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
        std::vector<libserial::Device> devices;
        ports.getDevices(devices);

        std::cout << "\nRetrieving device list via getDevices() method:\n";
        for (const auto& device : devices) {
            std::cout << "Device Name: " << device.getName() << "\n";
            std::cout << "Port Path: " << device.getPortPath() << "\n";
            std::cout << "Bus Path: " << device.getBusPath() << "\n";
            std::cout << "Device ID: " << device.getId() << "\n";
        }

        return 0;
    }


Building and Running Examples
-----------------------------

To run the examples, without physical serial devices, you can create virtual serial ports using `socat`. Open a terminal and run:

.. code-block:: bash
    # Terminal 1: Create virtual serial ports
    $ socat -d -d pty,link=/tmp/ttyV0,raw,echo=0 pty,link=/tmp/ttyV1,raw,echo=0
    2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/2
    2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/3
    2022/09/09 11:13:10 socat[19050] N starting data transfer loop with FDs [5,5] and [7,7]

You can build the examples using the main CMake system:

.. code-block:: bash
    $ mkdir build && cd build
    $ cmake .. -DBUILD_EXAMPLES=ON
    $ make examples

To run a simple communication example, execute the basic_communication example:

.. code-block:: bash
    # Terminal 2: Run the example
    $ ./basic_communication /tmp/ttyV0

    # Terminal 3: Interact with the other end
    $ echo "Hello from terminal!" > /tmp/ttyV1
    $ cat /tmp/ttyV1


You can also run the list_ports example to display all available serial devices on your system:

.. code-block:: bash
    ./examples/list_ports