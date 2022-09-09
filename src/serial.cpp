// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/serial.hpp"

serial::Serial::Serial() {
  std::cout << "Created Serial object" << std::endl;
}

serial::Serial::~Serial() {
  // Contructor
  close(fd_serial_port_);
}

void serial::Serial::open_port(std::string port) {
  fd_serial_port_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd_serial_port_ == -1) {
    printf("Error opening port %s: %s", port.data(), strerror(errno));
  } else {
    fcntl(fd_serial_port_, F_SETFL, 0);
    std::cout << "Open port"  << port << std::endl;
  }
}

void serial::Serial::send_msg(std::shared_ptr<std::string> msg_ptr) {
  n_ = write(fd_serial_port_ , msg_ptr.get()->c_str(), msg_ptr.get()->length());
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  n_ = write(fd_serial_port_ , "\n", 1);
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}

std::string serial::Serial::receive_msg() {
  char read_buf[kLengthBuffer_];
  std::string message;
  memset(read_buf, '\0', sizeof(read_buf));
  n_ = read(fd_serial_port_, &read_buf, sizeof(read_buf));
  message = read_buf;
  message.pop_back();
  return message;
}
