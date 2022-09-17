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
  n_ = write(fd_serial_port_ , msg_ptr->c_str(), msg_ptr->length());
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  n_ = write(fd_serial_port_ , "\n", 1);
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}

void serial::Serial::receive_msg(std::string* msg_ptr) {
  char *read_buf =
                reinterpret_cast<char *>(malloc(sizeof(char)*kLengthBuffer_));
  memset(read_buf, '\0', kLengthBuffer_);
  n_ = read(fd_serial_port_, read_buf, kLengthBuffer_);
  *msg_ptr = read_buf;
}
