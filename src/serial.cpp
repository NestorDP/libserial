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
  terminator_ = (int) Terminator::CR;
}

Serial::Serial(std::string port) {
  std::cout << "Created Serial object" << std::endl;
  terminator_ = (int) Terminator::CR;
  this->OpenPort(port);
}

Serial::~Serial() {
  close(fd_serial_port_);
}

void Serial::OpenPort(std::string port) {
  fd_serial_port_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (fd_serial_port_ == -1) {
    throw SerialException("Error opening port " + port + ": " + strerror(errno));
  } else {
    fcntl(fd_serial_port_, F_SETFL, 0);
    std::cout << "Open port"  << port << std::endl;
  }
}

void Serial::SendMsg(std::string *msg_ptr) {
  error_ = write(fd_serial_port_ , msg_ptr->c_str(), msg_ptr->length());
  std::cout << msg_ptr->data() << std::endl;
  if (error_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
  error_ = write(fd_serial_port_ , "\r", 1);
  if (error_ < 0) {
    printf("Error send mensage: %s", strerror(errno));
  }
}

void Serial::ReceiveMsg(std::string* msg_ptr) {
  char *read_buf =
                reinterpret_cast<char *>(malloc(kLengthBuffer_*sizeof(char)));
  memset(read_buf, '\0', kLengthBuffer_);
  error_ = read(fd_serial_port_, read_buf, kLengthBuffer_);
  *msg_ptr = read_buf;
  ioctl(fd_serial_port_, TCFLSH, 0);
}

void Serial::GetTermios2() {
  error_ = ioctl(fd_serial_port_, TCGETS2, &options_);
  if (error_ < 0) {
    printf("Error get Termios2: %s", strerror(errno));
  }
}

void Serial::SetTermios2() {
  error_ = ioctl(fd_serial_port_, TCSETS2, &options_);
  if (error_ < 0) {
    printf("Error set Termios2: %s", strerror(errno));
  }
}

void Serial::SetNumberBits(NumBits num_bits) {
  this->GetTermios2();

  // Clear bits
  options_.c_cflag &= ~CSIZE;

  switch (num_bits) {
    case NumBits::FIVE:
      options_.c_cflag |= CS5;
      break;
    case NumBits::SIX:
      options_.c_cflag |= CS6;
      break;
    case NumBits::SEVEN:
      options_.c_cflag |= CS7;
      break;
    case NumBits::EIGHT:
      options_.c_cflag |= CS8;
      break;
    default:
      options_.c_cflag |= CS8;
      break;
  }
  this->SetTermios2();
}

void Serial::SetParity(Parity parity) {
  this->GetTermios2();
  switch (parity) {
    case Parity::DISABLE:
      options_.c_cflag &= ~PARENB;
      break;
    case Parity::ENABLE:
      options_.c_cflag |= PARENB;
      break;
    default:
      options_.c_cflag &= ~PARENB;
      break;
  }
  this->SetTermios2();
}

void Serial::SetTwoStopBits(StopBits stop_bits) {
  this->GetTermios2();
  switch (stop_bits) {
  case StopBits::DISABLE:
    options_.c_cflag &= ~CSTOP;
    break;
  case StopBits::ENABLE:
    options_.c_cflag |= CSTOP;
  default:
    options_.c_cflag &= ~CSTOP;
    break;
  }
  this->SetTermios2();
}

void Serial::SetFlowControl(FlowControl flow_control) {
  this->GetTermios2();
  switch (flow_control) {
  case FlowControl::Software:
    // options_.c_cflag &= ~CRTSCTS;
    // options_.c_oflag |= (OPOST | ONLCR);
    // options_.c_iflag |= (IXON | IXOFF );
  options_.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  options_.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  options_.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  options_.c_cflag |= CS8; // 8 bits per byte (most common)
  options_.c_cflag &= ~CRTSCTS; // DISABLE RTS/CTS hardware flow control (most common)
  options_.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  options_.c_lflag &= ~ICANON;
  options_.c_lflag &= ~ECHO; // DISABLE echo
  options_.c_lflag &= ~ECHOE; // DISABLE erasure
  options_.c_lflag &= ~ECHONL; // DISABLE new-line echo
  options_.c_lflag &= ~ISIG; // DISABLE interpretation of INTR, QUIT and SUSP
  options_.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  options_.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // DISABLE any special handling of received bytes

  options_.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  options_.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // options_.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // options_.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  options_.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  options_.c_cc[VMIN] = 0;
  // options_.c_cc[VEOF] = '\r';

    break;
  case FlowControl::Hardware:
    options_.c_cflag |= CRTSCTS;
    options_.c_iflag &= ~(IXON | IXOFF | IXANY);    
  default:
    options_.c_cflag &= ~CRTSCTS;
    break;
  }
  this->SetTermios2();
}

void Serial::SetCanonicalMode(CanonicalMode canonical_mode){
  this->GetTermios2();
  switch (canonical_mode) {
  case CanonicalMode::ENABLE:
    options_.c_lflag |=  (ICANON);
    // options_.c_lflag &= ~(ECHOE | ECHO | ECHONL);
    options_.c_cc[VEOF] = '\n';
    break;
  case CanonicalMode::DISABLE:
    options_.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHONL);
    break;
  }
  this->SetTermios2();
}

void Serial::SetTerminator(Terminator term) {
  terminator_ = (int)term;
}

void Serial::SetTimeOut(int time){
  this->GetTermios2();
  options_.c_cc[VTIME] = time;
  this->SetTermios2();
}

void Serial::SetMinNumberCharRead(int num) {
  this->GetTermios2();
  options_.c_cc[VMIN] = num;
  this->SetTermios2();
}
}  // namespace serial
