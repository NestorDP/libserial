Examples
========

This section provides practical examples of using libserial in various scenarios.

Basic Serial Communication
---------------------------

Simple Echo Server
~~~~~~~~~~~~~~~~~~

Create a simple echo server that reads data and sends it back:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   #include <string>

   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.setBaudRate(libserial::BaudRate::BAUD_9600);
           serial.open();

           std::cout << "Echo server started. Press Ctrl+C to exit." << std::endl;

           while (true) {
               if (serial.available() > 0) {
                   std::string data = serial.read();
                   std::cout << "Received: " << data << std::endl;
                   serial.write("Echo: " + data);
               }
               
               // Small delay to prevent busy waiting
               std::this_thread::sleep_for(std::chrono::milliseconds(10));
           }

       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }

Data Logger
~~~~~~~~~~~

Log incoming serial data to a file:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   #include <fstream>
   #include <chrono>
   #include <iomanip>

   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.setBaudRate(libserial::BaudRate::BAUD_115200);
           serial.setTimeout(std::chrono::seconds(1));
           serial.open();

           std::ofstream logFile("serial_log.txt");
           
           std::cout << "Data logger started. Logging to serial_log.txt" << std::endl;

           while (true) {
               try {
                   std::string data = serial.read(256);
                   
                   // Get current timestamp
                   auto now = std::chrono::system_clock::now();
                   auto time_t = std::chrono::system_clock::to_time_t(now);
                   
                   // Log with timestamp
                   logFile << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
                          << " - " << data << std::endl;
                   logFile.flush();
                   
                   std::cout << "Logged: " << data << std::endl;
                   
               } catch (const libserial::TimeoutException& e) {
                   // Timeout is expected, continue
                   continue;
               }
           }

       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }

Device Communication
--------------------

AT Command Interface
~~~~~~~~~~~~~~~~~~~~

Communicate with AT command-based devices (modems, GPS modules, etc.):

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   #include <string>
   #include <chrono>

   class ATDevice {
   private:
       libserial::Serial serial_;
       
   public:
       ATDevice(const std::string& port) : serial_(port) {
           serial_.setBaudRate(libserial::BaudRate::BAUD_115200);
           serial_.setTimeout(std::chrono::seconds(5));
       }
       
       void connect() {
           serial_.open();
           std::cout << "Connected to device" << std::endl;
       }
       
       std::string sendCommand(const std::string& command) {
           // Send command with CR+LF
           serial_.write(command + "\r\n");
           
           // Read response
           std::string response;
           try {
               response = serial_.read(1024);
           } catch (const libserial::TimeoutException& e) {
               throw std::runtime_error("Command timeout: " + command);
           }
           
           return response;
       }
       
       bool testConnection() {
           try {
               std::string response = sendCommand("AT");
               return response.find("OK") != std::string::npos;
           } catch (const std::exception& e) {
               return false;
           }
       }
   };

   int main() {
       try {
           ATDevice device("/dev/ttyUSB0");
           device.connect();
           
           if (device.testConnection()) {
               std::cout << "Device is responding" << std::endl;
               
               // Get device information
               std::string info = device.sendCommand("ATI");
               std::cout << "Device info: " << info << std::endl;
               
           } else {
               std::cout << "Device is not responding" << std::endl;
           }

       } catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }

Sensor Data Collection
~~~~~~~~~~~~~~~~~~~~~~

Collect data from a serial sensor:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   #include <sstream>
   #include <vector>

   struct SensorReading {
       double temperature;
       double humidity;
       double pressure;
   };

   class SensorReader {
   private:
       libserial::Serial serial_;
       
   public:
       SensorReader(const std::string& port) : serial_(port) {
           serial_.setBaudRate(libserial::BaudRate::BAUD_9600);
           serial_.setTimeout(std::chrono::seconds(2));
       }
       
       void connect() {
           serial_.open();
       }
       
       SensorReading readSensor() {
           // Request sensor data
           serial_.write("READ\n");
           
           // Read response (assuming CSV format: temp,humidity,pressure)
           std::string response = serial_.read(100);
           
           // Parse CSV data
           std::istringstream ss(response);
           std::string token;
           std::vector<double> values;
           
           while (std::getline(ss, token, ',')) {
               values.push_back(std::stod(token));
           }
           
           if (values.size() != 3) {
               throw std::runtime_error("Invalid sensor data format");
           }
           
           return {values[0], values[1], values[2]};
       }
   };

   int main() {
       try {
           SensorReader sensor("/dev/ttyUSB0");
           sensor.connect();
           
           for (int i = 0; i < 10; ++i) {
               SensorReading reading = sensor.readSensor();
               
               std::cout << "Reading " << (i + 1) << ": "
                        << "Temp=" << reading.temperature << "°C, "
                        << "Humidity=" << reading.humidity << "%, "
                        << "Pressure=" << reading.pressure << "hPa"
                        << std::endl;
               
               std::this_thread::sleep_for(std::chrono::seconds(1));
           }

       } catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }

Port Discovery
--------------

Finding Available Ports
~~~~~~~~~~~~~~~~~~~~~~~~

Discover and list available serial ports:

.. code-block:: cpp

   #include <libserial/ports.hpp>
   #include <iostream>

   int main() {
       try {
           // Get list of available ports
           auto ports = libserial::getAvailablePorts();
           
           std::cout << "Available serial ports:" << std::endl;
           
           if (ports.empty()) {
               std::cout << "No serial ports found." << std::endl;
           } else {
               for (const auto& port : ports) {
                   std::cout << "  " << port << std::endl;
               }
           }
           
           // Try to open the first available port
           if (!ports.empty()) {
               libserial::Serial serial(ports[0]);
               serial.open();
               std::cout << "Successfully opened: " << ports[0] << std::endl;
               serial.close();
           }

       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }

Building and Running Examples
-----------------------------

To build these examples, create a CMakeLists.txt file:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.10)
   project(libserial_examples)

   set(CMAKE_CXX_STANDARD 14)

   find_package(PkgConfig REQUIRED)
   pkg_check_modules(LIBSERIAL REQUIRED libserial)

   # Echo server example
   add_executable(echo_server echo_server.cpp)
   target_link_libraries(echo_server ${LIBSERIAL_LIBRARIES})

   # Data logger example
   add_executable(data_logger data_logger.cpp)
   target_link_libraries(data_logger ${LIBSERIAL_LIBRARIES})

   # AT device example
   add_executable(at_device at_device.cpp)
   target_link_libraries(at_device ${LIBSERIAL_LIBRARIES})

Then build:

.. code-block:: bash

   mkdir build && cd build
   cmake ..
   make