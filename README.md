# ⚙️ CppSerial

**C++ Application Programming Interface for the `asm/termbits.h` C library**

---

The goal of this API is to provide developers with a modern C++ interface to the traditional C `asm/termbits.h` library.  
It uses **Object-Oriented Programming concepts**, **smart pointers**, **lambda functions**, and more to give your applications greater flexibility and power when dealing with **serial communication** in a **Linux environment**.

---

## ✨ Features

- 🚀 Modern C++ API for serial communication  
- 🧠 Object-Oriented design using smart pointers and RAII  
- ⚙️ Full control of serial port configuration via `termios2`  
- 🧩 Example applications included  
- 🐧 Works only on Linux systems

---

## 🛠️ Build and Installation


```bash
# Clone the repository:
git clone https://github.com/NestorDP/libserial.git && cd libserial

# Build the source code:
mkdir build && cd build
cmake ..
make

# Install the library (you will need to enter your sudo password):
sudo make install
```

## 🧪 Run an Example Application

You can run an example application to test the libserial library in your environment.
Even without a physical serial device, you can test serial communication using a virtual serial port created with socat.

### 📦 Install socat (for virtual serial ports)

```bash
# Ubuntu/Debian
sudo apt-get install socat

# CentOS/RHEL/Fedora
sudo yum install socat    # or: sudo dnf install socat
```

### 🔧 Create Virtual Serial Ports

Open a new terminal and run:

```bash
# Terminal 1: Create virtual serial ports
socat -d -d pty,link=/tmp/ttyV0,raw,echo=0 pty,link=/tmp/ttyV1,raw,echo=0
2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/2
2022/09/09 11:13:10 socat[19050] N PTY is /dev/pts/3
2022/09/09 11:13:10 socat[19050] N starting data transfer loop with FDs [5,5] and [7,7]
```

After this, your system will have two connected virtual serial ports.
In this example, /dev/pts/2 and /dev/pts/3 are linked to /tmp/ttyV0 and /tmp/ttyV1, respectively.

### 🧰 Building Examples

You can build the examples using the main CMake system:

```bash
cmake .. -DBUILD_EXAMPLES=ON
make examples
```

Or compile them individually:

```bash
g++ -std=c++14 -I../include examples/basic_communication.cpp -L../build -llibserial -o basic_comm
```

### ▶️ Run Examples

To run a simple communication example, execute the basic_communication example:

```bash
# Terminal 2: Run the example
./basic_communication /tmp/ttyV0

# Terminal 3: Interact with the other end
echo "Hello from terminal!" > /tmp/ttyV1
cat /tmp/ttyV1
```

You can also run the list_ports example to display all available serial devices on your system:

```bash
./examples/list_ports
```

### 🤝 Contributing

Contributions, issues, and feature requests are welcome!
Feel free to open an issue or submit a pull request to help improve this project.

Before submitting a PR, please:

- Follow the existing coding style
- Add or update documentation when necessary
- Include tests for new features if possible

### 📄 License

This project is licensed under the GNU General Public License v3.0 (GPLv3).
See the [LICENSE](./LICENSE) file for more details.
