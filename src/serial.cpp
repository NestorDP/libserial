// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/serial.hpp"

namespace serial {
  
Serial::Serial() {
  std::cout << "Created Serial object" << std::endl;
}

Serial::Serial(std::string port) {
  std::cout << "Created Serial object" << std::endl;
  this->open(port);
}

Serial::~Serial() {
  this->close();
}

void Serial::open(std::string port) {
  fd_serial_port_ = ::open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  
  if (fd_serial_port_ == -1) {
    throw SerialException("Error opening port " + port + ": " + strerror(errno));
  } else {
    fcntl(fd_serial_port_, F_SETFL, 0);
    std::cout << "Open port"  << port << std::endl;
  }
}

void Serial::close() {
  ssize_t error = ::close(fd_serial_port_);
  if (error < 0) {
    throw SerialException("Error closing port: " + std::string(strerror(errno)));
  }
}

void Serial::write(std::string& data) {
  data.push_back('\r');

  ssize_t bytes_written = ::write(fd_serial_port_, data.c_str(), data.size());

  if (bytes_written < 0) {
    throw SerialException("Error writing to serial port: " + std::string(strerror(errno)));
  }
}

std::string Serial::read(size_t max_length) {
  std::vector<char> buffer(max_length);

  ssize_t bytes_read = ::read(fd_serial_port_, buffer.data(), max_length);
  if (bytes_read < 0) {
    throw SerialException("Error reading from serial port: " + std::string(strerror(errno)));
  }

  return std::string(buffer.begin(), buffer.begin() + bytes_read);
}

std::string Serial::readUntil(char terminator) {
  std::vector<char> buffer(1);
  std::string data;

  while (buffer[0] != terminator) {
    ssize_t bytes_read = ::read(fd_serial_port_, buffer.data(), 1);
    if (bytes_read < 0) {
      throw SerialException("Error reading from serial port: " + std::string(strerror(errno)));
    }
    data.push_back(buffer[0]);
  }
  return data;
}

void Serial::flushInputBuffer() {
  if (ioctl(fd_serial_port_, TCFLSH, TCIFLUSH) != 0) {
    throw SerialException("Error flushing input buffer: " + std::string(strerror(errno)));
  }
}

int Serial::getAvailableData() const {
  int bytes_available;
  if (ioctl(fd_serial_port_, FIONREAD, &bytes_available) < 0){
    throw SerialException("Error getting available data: " + std::string(strerror(errno)));
  }
  return bytes_available;
}

void Serial::setBaudRate(int baud_rate) {
  this->GetTermios2();
  options_.c_cflag &= ~CBAUD;
  options_.c_cflag |= BOTHER;
  options_.c_ispeed = baud_rate;
  options_.c_ospeed = baud_rate;
  this->SetTermios2();
}

void Serial::GetTermios2() {
  ssize_t error = ioctl(fd_serial_port_, TCGETS2, &options_);
  if (error < 0) {
    throw SerialException("Error get Termios2: " + std::string(strerror(errno)));
  }
}

void Serial::SetTermios2() {
  ssize_t error = ioctl(fd_serial_port_, TCSETS2, &options_);
  if (error < 0) {
    throw SerialException("Error set Termios2: " + std::string(strerror(errno)));
  }
}

// void Serial::SetNumberBits(NumBits num_bits) {
//   this->GetTermios2();

//   // Clear bits
//   options_.c_cflag &= ~CSIZE;

//   switch (num_bits) {
//     case NumBits::FIVE:
//       options_.c_cflag |= CS5;
//       break;
//     case NumBits::SIX:
//       options_.c_cflag |= CS6;
//       break;
//     case NumBits::SEVEN:
//       options_.c_cflag |= CS7;
//       break;
//     case NumBits::EIGHT:
//       options_.c_cflag |= CS8;
//       break;
//     default:
//       options_.c_cflag |= CS8;
//       break;
//   }
//   this->SetTermios2();
// }

// void Serial::SetParity(Parity parity) {
//   this->GetTermios2();
//   switch (parity) {
//     case Parity::DISABLE:
//       options_.c_cflag &= ~PARENB;
//       break;
//     case Parity::ENABLE:
//       options_.c_cflag |= PARENB;
//       break;
//     default:
//       options_.c_cflag &= ~PARENB;
//       break;
//   }
//   this->SetTermios2();
// }

// void Serial::SetTwoStopBits(StopBits stop_bits) {
//   this->GetTermios2();
//   switch (stop_bits) {
//   case StopBits::DISABLE:
//     options_.c_cflag &= ~CSTOP;
//     break;
//   case StopBits::ENABLE:
//     options_.c_cflag |= CSTOP;
//   default:
//     options_.c_cflag &= ~CSTOP;
//     break;
//   }
//   this->SetTermios2();
// }

// void Serial::SetFlowControl(FlowControl flow_control) {
//   this->GetTermios2();
//   switch (flow_control) {
//   case FlowControl::Software:
//     // options_.c_cflag &= ~CRTSCTS;
//     // options_.c_oflag |= (OPOST | ONLCR);
//     // options_.c_iflag |= (IXON | IXOFF );
//   options_.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
//   options_.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
//   options_.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
//   options_.c_cflag |= CS8; // 8 bits per byte (most common)
//   options_.c_cflag &= ~CRTSCTS; // DISABLE RTS/CTS hardware flow control (most common)
//   options_.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

//   options_.c_lflag &= ~ICANON;
//   options_.c_lflag &= ~ECHO; // DISABLE echo
//   options_.c_lflag &= ~ECHOE; // DISABLE erasure
//   options_.c_lflag &= ~ECHONL; // DISABLE new-line echo
//   options_.c_lflag &= ~ISIG; // DISABLE interpretation of INTR, QUIT and SUSP
//   options_.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
//   options_.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // DISABLE any special handling of received bytes

//   options_.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
//   options_.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
//   // options_.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
//   // options_.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

//   options_.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
//   options_.c_cc[VMIN] = 0;
//   // options_.c_cc[VEOF] = '\r';

//     break;
//   case FlowControl::Hardware:
//     options_.c_cflag |= CRTSCTS;
//     options_.c_iflag &= ~(IXON | IXOFF | IXANY);    
//   default:
//     options_.c_cflag &= ~CRTSCTS;
//     break;
//   }
//   this->SetTermios2();
// }

// void Serial::SetCanonicalMode(CanonicalMode canonical_mode){
//   this->GetTermios2();
//   switch (canonical_mode) {
//   case CanonicalMode::ENABLE:
//     options_.c_lflag |=  (ICANON);
//     // options_.c_lflag &= ~(ECHOE | ECHO | ECHONL);
//     options_.c_cc[VEOF] = '\n';
//     break;
//   case CanonicalMode::DISABLE:
//     options_.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHONL);
//     break;
//   }
//   this->SetTermios2();
// }

// void Serial::SetTerminator(Terminator term) {
//   terminator_ = (int)term;
// }

// void Serial::SetTimeOut(int time){
//   this->GetTermios2();
//   options_.c_cc[VTIME] = time;
//   this->SetTermios2();
// }

// void Serial::SetMinNumberCharRead(int num) {
//   this->GetTermios2();
//   options_.c_cc[VMIN] = num;
//   this->SetTermios2();
// }
}  // namespace serial
