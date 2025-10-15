# CPPserial
C++ Application Programming Interface to termios.h C library 

The goal of this API is provide to developers a way to use the old C's library,  termios.h, lthrough a modern C++ class. Concepts as Oriented-Object Programming, Smart Pointers, lambda function and more are using here to give you more flexibilit and powerfull to your aplication that need serial communication in a Linux enviroment. 


## Build and installation
Clone the repository.
``` bash
git clone https://github.com/NestorDP/libserial.git && cd libserial
```

Building the source code.
``` bash
mkdir build && cd build
cmake ..
make
```

Install the library. You will to need put your sudo password to finish this step.
``` bash
make install
```

## Run an example application
You can run an example application to test the libserial library in your environment. Even without a hardware device you can to test the serial communication with a virtual serial port using the *socat* for to create an pair of virtual ports.

### Install socat (for virtual serial port)
```bash
# Ubuntu/Debian
sudo apt-get install socat

# CentOS/RHEL/Fedora
sudo yum install socat    # or dnf install socat
```

### Create a virtual serial port


To create the virtual ports pair, open a new bash and run this command:
``` bash
# Terminal 1: Create virtual serial ports
socat -d -d pty,link=/tmp/ttyV0,raw,echo=0 pty,link=/tmp/ttyV1,raw,echo=0
2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/2
2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/3
2022/09/09 11:13:10 socat[19050] N starting data transfer loop with FDs [5,5] and [7,7]
```
After this, your system have two serial ports virtuals connected, in this case we have the /dev/pts/2 and /dev/pts/3 wich you can access through the links /tmp/ttyV0 and /tmp/ttyV1.

### Building Examples

The examples can be built using the main CMake system:

```bash
cmake .. -DBUILD_EXAMPLES=ON
make examples
```

Or compile individually:
```bash
g++ -std=c++14 -I../include examples/basic_communication.cpp -L../build -llibserial -o basic_communication
```
### Run 
For a simple communication aplication run the basic_communication example
```bash
# Terminal 2: Run example
./basic_communication /tmp/ttyV0

# Terminal 3: Interact with the other end
echo "Hello from terminal!" > /tmp/ttyV1
cat /tmp/ttyV1
```

You can run the list_port example to show all serial devices avaliable in your system
``` bash
./examples/list_ports
```
