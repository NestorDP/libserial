Welcome to CppSerial's documentation!
=====================================

CppSerial is a C++ library for serial communication on Linux systems. It provides a simple and intuitive interface for opening, configuring, and communicating through serial ports.

The goal of this API is to provide developers with a modern C++ interface to the traditional C header asm/termbits.h.
It allows setting any baud rate supported by the hardware, not just the standard ones, enabling a more flexible communication process that is ideal for industrial or embedded applications requiring non-standard baud rates.
It uses **Object-Oriented Programming concepts**, **smart pointers**, **lambda functions**, and more to give your applications greater flexibility and power when dealing with **serial communication** in a **Linux environment**.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   installation
   usage
   api_reference
   examples

Features
--------

* Simple C++ interface for serial operations
* Support for various baud rates and communication parameters
* Modern C++17 implementation
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

.. Indices and tables
.. ==================

.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`