Usage Guide
===========

Basic Usage
-----------

The libserial library provides a simple C++ interface for serial port communication. Here's how to get started:

Opening a Serial Port
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #include <libserial/serial.hpp>
   
   int main() {
       try {
           // Create a Serial object
           libserial::Serial serial("/dev/ttyUSB0");
           
           // Open the port
           serial.open();
           
           // Port is now ready for communication
           
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }
       
       return 0;
   }

Configuring Serial Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can configure various serial port parameters:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   
   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           
           // Configure before opening
           serial.setBaudRate(libserial::BaudRate::BAUD_9600);
           serial.setDataBits(libserial::DataBits::DATA_8);
           serial.setParity(libserial::Parity::NONE);
           serial.setStopBits(libserial::StopBits::STOP_1);
           
           serial.open();
           
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }
       
       return 0;
   }

Reading and Writing Data
~~~~~~~~~~~~~~~~~~~~~~~~

Basic read/write operations:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   #include <string>
   
   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.open();
           
           // Write data
           std::string message = "Hello, Serial!";
           serial.write(message);
           
           // Read response
           std::string response = serial.read(100); // Read up to 100 bytes
           std::cout << "Received: " << response << std::endl;
           
           // Read a single byte
           char byte = serial.readByte();
           std::cout << "Single byte: " << static_cast<int>(byte) << std::endl;
           
           serial.close();
           
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }
       
       return 0;
   }

Advanced Usage
--------------

Timeout Configuration
~~~~~~~~~~~~~~~~~~~~~

Configure read timeouts to prevent blocking:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <chrono>
   
   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.open();
           
           // Set read timeout to 5 seconds
           serial.setTimeout(std::chrono::seconds(5));
           
           // This will timeout after 5 seconds if no data is available
           std::string data = serial.read(100);
           
       } catch (const libserial::TimeoutException& e) {
           std::cout << "Read operation timed out" << std::endl;
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
       }
       
       return 0;
   }

Asynchronous Operations
~~~~~~~~~~~~~~~~~~~~~~~

For non-blocking operations:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   
   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.open();
           
           // Check if data is available before reading
           if (serial.available() > 0) {
               std::string data = serial.read();
               // Process data...
           }
           
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
       }
       
       return 0;
   }

Error Handling
--------------

libserial uses exceptions for error handling. The main exception types are:

* ``SerialException``: Base exception class
* ``PortNotFoundException``: Port doesn't exist or can't be accessed
* ``PermissionDeniedException``: Insufficient permissions
* ``TimeoutException``: Operation timed out
* ``IOException``: Input/output error

.. code-block:: cpp

   try {
       // Serial operations...
   } catch (const libserial::PortNotFoundException& e) {
       std::cerr << "Port not found: " << e.what() << std::endl;
   } catch (const libserial::PermissionDeniedException& e) {
       std::cerr << "Permission denied: " << e.what() << std::endl;
   } catch (const libserial::TimeoutException& e) {
       std::cerr << "Operation timed out: " << e.what() << std::endl;
   } catch (const libserial::SerialException& e) {
       std::cerr << "Serial error: " << e.what() << std::endl;
   }

Best Practices
--------------

1. **Always use exception handling** when working with serial ports
2. **Close ports explicitly** or use RAII patterns
3. **Set appropriate timeouts** to prevent blocking
4. **Check data availability** before reading in loops
5. **Use appropriate buffer sizes** for your application
6. **Test with different baud rates** to find optimal settings