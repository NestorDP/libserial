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
  close(fdSerialPort_);
}

void serial::Serial::open_port(std::string port) {
  fdSerialPort_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fdSerialPort_ == -1) {
    printf("Error opening port %s: %s", port.data(), strerror(errno));
  } else {
    fcntl(fdSerialPort_, F_SETFL, 0);
    std::cout << "Open port"  << port << std::endl;
  }
}

void serial::Serial::send_msg(std::shared_ptr<std::string> msg_ptr) {
  n_ = write(fdSerialPort_ , msg_ptr->data(), msg_ptr->length());
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  n_ = write(fdSerialPort_ , "\n", 1);
  if (n_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}

void serial::Serial::receive_msg(std::string* msg_ptr) {
  char *read_buf =
                reinterpret_cast<char *>(malloc(kLengthBuffer_*sizeof(char)));
  memset(read_buf, '\0', kLengthBuffer_);
  n_ = read(fdSerialPort_, read_buf, kLengthBuffer_);
  *msg_ptr = read_buf;
}
