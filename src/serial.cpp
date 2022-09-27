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
  close(fd_serial_port_);
}


void serial::Serial::OpenPort(std::string port) {
  fd_serial_port_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd_serial_port_ == -1) {
    printf("Error opening port %s: %s", port.data(), strerror(errno));
  } else {
    fcntl(fd_serial_port_, F_SETFL, 0);
    std::cout << "Open port"  << port << std::endl;
  }
}


void serial::Serial::SendMsg(std::shared_ptr<std::string> msg_ptr) {
  n_ = write(fd_serial_port_ , msg_ptr->c_str(), msg_ptr->length());
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  n_ = write(fd_serial_port_ , "\n", 1);
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}


void serial::Serial::ReceiveMsg(std::string* msg_ptr) {
  char *read_buf =
                reinterpret_cast<char *>(malloc(kLengthBuffer_*sizeof(char)));
  memset(read_buf, '\0', kLengthBuffer_);
  n_ = read(fd_serial_port_, read_buf, kLengthBuffer_);
  *msg_ptr = read_buf;
}


void serial::Serial::GetTermios2(struct termio2 *tty) {
  ioctl(fd_serial_port_, TCGETS2, tty);
}

void serial::Serial::SetTermios2(struct termio2 *tty) {
  ioctl(fd_serial_port_, TCSETS2, tty);
}


void serial::Serial::SetNumberBits(int num_bits) {

  tty_.c_cflag &= ~CSIZE;
  switch (num_bits) {
  case 5:
    tty_.c_cflag |= CS5;
    break;
  case 6:
    tty_.c_cflag |= CS6;
    break;
  case 7:
    tty_.c_cflag |= CS7;
    break;
  case 8:
    tty_.c_cflag |= CS8;
    break;
  default:
    tty_.c_cflag |= CS8;
  }

}


