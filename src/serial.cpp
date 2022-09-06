/**
 * @ Copyright 2020
 */

#include "libserial/serial.hpp"

serial::Serial::Serial() {
  printf("open object");
}

serial::Serial::~Serial() {
  close(fd_serial_port_);
}

void serial::Serial::open_port(std::string port) {
  fd_serial_port_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd_serial_port_ == -1) {
    printf("Error opening port %s: %s", port.data(), strerror(errno));
  } else {
    fcntl(fd_serial_port_, F_SETFL, 0);
    printf("Open port %s", port.data());
  }
}

void serial::Serial::send_msg(std::string msg) {
  n_ = write(fd_serial_port_ , msg.c_str(), msg.length());
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  n_ = write(fd_serial_port_ , "\n", 1);
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}

void serial::Serial::receive_msg(std::string msg) {
  char read_buf[kLengthBuffer_];
  memset(read_buf, '\0', sizeof(read_buf));
  n_ = read(fd_serial_port_, &read_buf, sizeof(read_buf));
  msg = read_buf;
  msg.pop_back();
}
