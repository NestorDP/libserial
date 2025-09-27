Welcome to libserial's documentation!
=====================================

libserial is a C++ library for serial port communication on Linux systems. It provides a simple and intuitive interface for opening, configuring, and communicating through serial ports.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   installation
   usage
   api_reference
   examples

Features
--------

* Simple C++ interface for serial port operations
* Support for various baud rates and communication parameters
* Cross-platform compatibility (Linux focus)
* Modern C++14 implementation
* Exception-based error handling

Quick Start
-----------

Here's a simple example of how to use libserial:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>

   int main() {
       try {
           libserial::Serial serial("/dev/ttyUSB0");
           serial.open();
           serial.write("Hello, World!");
           
           std::string response = serial.read();
           std::cout << "Received: " << response << std::endl;
           
           serial.close();
       } catch (const libserial::SerialException& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }
       
       return 0;
   }

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`