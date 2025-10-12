# Examples Directory

This directory contains practical examples demonstrating how to use the libserial library with real and virtual serial ports.

## Prerequisites

### Install socat (for virtual serial port examples)
```bash
# Ubuntu/Debian
sudo apt-get install socat

# CentOS/RHEL/Fedora
sudo yum install socat    # or dnf install socat
```

## Examples

### 1. **basic_communication.cpp**
Simple send/receive example using socat virtual ports

### 2. **bidirectional_chat.cpp** 
Two-way communication example

### 3. **baud_rate_testing.cpp**
Testing different baud rates with virtual ports

### 4. **error_handling.cpp**
Demonstrates proper error handling and exception management

## Running Examples

### Method 1: Using provided scripts
```bash
cd examples
./run_basic_example.sh
```

### Method 2: Manual setup
```bash
# Terminal 1: Create virtual serial ports
socat -d -d pty,link=/tmp/ttyV0,raw,echo=0 pty,link=/tmp/ttyV1,raw,echo=0

# Terminal 2: Run example
./basic_communication /tmp/ttyV0

# Terminal 3: Interact with the other end
echo "Hello from terminal!" > /tmp/ttyV1
cat /tmp/ttyV1
```

## Building Examples

The examples can be built using the main CMake system:

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
make examples
```

Or compile individually:
```bash
g++ -std=c++14 -I../include examples/basic_communication.cpp -L../build -llibserial -o basic_communication
```