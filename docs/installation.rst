Installation
============

Building from Source
---------------------

Prerequisites
~~~~~~~~~~~~~

To build libserial from source, you'll need:

* CMake (version 3.10 or higher)
* A C++14 compatible compiler (GCC 5.4+ or Clang 3.4+)
* Git (for cloning the repository)

For documentation generation (optional):

* Doxygen
* Python 3.6+
* Sphinx
* sphinx-rtd-theme
* breathe

Building the Library
~~~~~~~~~~~~~~~~~~~~

1. Clone the repository:

.. code-block:: bash

   git clone https://github.com/NestorDP/libserial.git
   cd libserial

2. Create a build directory:

.. code-block:: bash

   mkdir build
   cd build

3. Configure the build:

.. code-block:: bash

   cmake ..

4. Build the library:

.. code-block:: bash

   make

5. Install the library (optional):

.. code-block:: bash

   sudo make install

Using CMake Options
~~~~~~~~~~~~~~~~~~~

You can customize the build with various CMake options:

.. code-block:: bash

   # Build in Release mode (optimized)
   cmake -DCMAKE_BUILD_TYPE=Release ..
   
   # Install to a custom location
   cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

Package Installation
--------------------

Ubuntu/Debian
~~~~~~~~~~~~~

.. code-block:: bash

   sudo apt update
   sudo apt install libserial-dev

(Note: Package availability depends on your distribution)

From Binary Releases
~~~~~~~~~~~~~~~~~~~~~

Download the latest release from the `GitHub releases page <https://github.com/NestorDP/libserial/releases>`_ and follow the installation instructions provided with the release.

Verifying Installation
----------------------

To verify that libserial is installed correctly, create a simple test program:

.. code-block:: cpp

   #include <libserial/serial.hpp>
   #include <iostream>
   
   int main() {
       std::cout << "libserial is installed!" << std::endl;
       return 0;
   }

Compile and run:

.. code-block:: bash

   g++ -std=c++14 test.cpp -lserial -o test
   ./test