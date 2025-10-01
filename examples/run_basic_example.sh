#!/bin/bash

# Script to run the basic communication example with socat

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== libserial Basic Communication Example ===${NC}"

# Check if socat is installed
if ! command -v socat &> /dev/null; then
    echo -e "${RED}Error: socat is not installed${NC}"
    echo "Please install socat:"
    echo "  Ubuntu/Debian: sudo apt-get install socat"
    exit 1
fi

# Check if example is built
if [ ! -f "../build/examples/basic_communication" ]; then
    echo -e "${YELLOW}Building example...${NC}"
    cd ../build
    make basic_communication
    cd ../examples
    
    if [ ! -f "../build/examples/basic_communication" ]; then
        echo -e "${RED}Error: Failed to build example${NC}"
        echo "Make sure you've built the library first:"
        echo "  cd build && cmake .. -DBUILD_EXAMPLES=ON && make"
        exit 1
    fi
fi

# Define virtual port names
PORT1="/tmp/ttyV0"
PORT2="/tmp/ttyV1"

# Clean up any existing virtual ports
rm -f $PORT1 $PORT2

echo -e "${YELLOW}Starting socat to create virtual serial ports...${NC}"
echo "Virtual ports: $PORT1 <-> $PORT2"

# Start socat in background
socat -d -d pty,link=$PORT1,raw,echo=0 pty,link=$PORT2,raw,echo=0 &
SOCAT_PID=$!

# Wait for virtual ports to be created
echo "Waiting for virtual ports to be ready..."
for i in {1..20}; do
    if [ -e $PORT1 ] && [ -e $PORT2 ]; then
        echo -e "${GREEN}Virtual ports created successfully!${NC}"
        break
    fi
    sleep 0.1
done

if [ ! -e $PORT1 ] || [ ! -e $PORT2 ]; then
    echo -e "${RED}Error: Failed to create virtual ports${NC}"
    kill $SOCAT_PID 2>/dev/null
    exit 1
fi

echo
echo -e "${BLUE}=== Instructions ===${NC}"
echo "1. The example program will connect to $PORT1"
echo "2. You can send data to $PORT2 from another terminal:"
echo "   echo 'Hello back!' > $PORT2"
echo "3. You can also read from $PORT2:"
echo "   cat $PORT2"
echo "4. Press Ctrl+C to exit"
echo

# Function to cleanup on exit
cleanup() {
    echo -e "\n${YELLOW}Cleaning up...${NC}"
    kill $SOCAT_PID 2>/dev/null
    rm -f $PORT1 $PORT2
    echo -e "${GREEN}Cleanup complete${NC}"
    exit 0
}

# Set up trap for cleanup
trap cleanup SIGINT SIGTERM

echo -e "${GREEN}Starting basic communication example...${NC}"
echo "Connect to the other end with: echo 'your message' > $PORT2"
echo

# Run the example
../build/examples/basic_communication $PORT1

# Cleanup
cleanup