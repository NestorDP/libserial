// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/serial.hpp"


serial::Serial::Serial() {
  std::cout << "Created Serial object" << std::endl;
  terminator_ = (int) serial::Terminator::CR;
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
  error_ = write(fd_serial_port_ , msg_ptr->c_str(), msg_ptr->length());
  if (error_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  error_ = write(fd_serial_port_ , "\n", 1);
  if (error_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}


void serial::Serial::ReceiveMsg(std::string* msg_ptr) {
  char *read_buf =
                reinterpret_cast<char *>(malloc(kLengthBuffer_*sizeof(char)));
  memset(read_buf, '\0', kLengthBuffer_);
  error_ = read(fd_serial_port_, read_buf, kLengthBuffer_);
  *msg_ptr = read_buf;
  ioctl(fd_serial_port_, TCFLSH);
}


void serial::Serial::GetTermios2() {
  error_ = ioctl(fd_serial_port_, TCGETS2, &options_);
  if (error_ < 0) {
    printf("Error get Termios2: %s", strerror(errno));
  }
}


void serial::Serial::SetTermios2() {
  error_ = ioctl(fd_serial_port_, TCSETS2, &options_);
  if (error_ < 0) {
    printf("Error set Termios2: %s", strerror(errno));
  }
}


void serial::Serial::SetNumberBits(NumBits num_bits) {
  this->GetTermios2();

  // Clear bits
  options_.c_cflag &= ~CSIZE;

  switch (num_bits) {
    case NumBits::kFive:
      options_.c_cflag |= CS5;
      break;
    case NumBits::kSix:
      options_.c_cflag |= CS6;
      break;
    case NumBits::kSeven:
      options_.c_cflag |= CS7;
      break;
    case NumBits::kEight:
      options_.c_cflag |= CS8;
      break;
    default:
      options_.c_cflag |= CS8;
      break;
  }
  this->SetTermios2();
}


void serial::Serial::SetParity(Parity parity) {
  this->GetTermios2();
  switch (parity) {
    case Parity::kDisable:
      options_.c_cflag &= ~PARENB;
      break;
    case Parity::kEnable:
      options_.c_cflag |= PARENB;
      break;
    default:
      options_.c_cflag &= ~PARENB;
      break;
  }
  this->SetTermios2();
}


void serial::Serial::SetTwoStopBits(StopBits stop_bits) {
  this->GetTermios2();
  switch (stop_bits) {
  case StopBits::kDisable:
    options_.c_cflag &= ~CSTOP;
    break;
  case StopBits::kEnable:
    options_.c_cflag |= CSTOP;
  default:
    options_.c_cflag &= ~CSTOP;
    break;
  }
  this->SetTermios2();
}


void serial::Serial::SetFlowControl(FlowControl flow_control) {
  this->GetTermios2();
  switch (flow_control) {
  case FlowControl::kSoftware:
    options_.c_cflag &= ~CRTSCTS;
    options_.c_iflag |= (IXON | IXOFF | IXANY);
    options_.c_cc[VEOF] = terminator_;     /* Ctrl-M CR */
    break;
  case FlowControl::kHardware:
    options_.c_cflag |= CRTSCTS;
    options_.c_iflag &= ~(IXON | IXOFF | IXANY);    
  default:
    options_.c_cflag &= ~CRTSCTS;
    break;
  }
  this->SetTermios2();
}

void serial::Serial::SetCanonicalMode(CanonicalMode canonical_mode){
  this->GetTermios2();
  switch (canonical_mode) {
  case CanonicalMode::kEnable:
    options_.c_lflag |=  (ICANON | ECHO | ECHOE);
    break;
  case CanonicalMode::kDisable:
    options_.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    break;
  }
  this->SetTermios2();
}

void serial::Serial::SetTerminator(Terminator term) {
  terminator_ = (int)term;
}

void serial::Serial::SetTimeOut(int time){
  this->GetTermios2();
  options_.c_cc[VTIME] = time;
  this->SetTermios2();
}

void serial::Serial::SetMinNumberCharRead(int num) {
  this->GetTermios2();
  options_.c_cc[VMIN] = num;
  this->SetTermios2();
}
