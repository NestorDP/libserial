// @ Copyright 2020

#include "libserial/serial.hpp"
#include <string>
#include <memory>
#include <poll.h>

namespace libserial {

Serial::Serial(const std::string& port) {
  this->open(port);
  this->setBaudRate(BaudRate::BAUD_RATE_9600);
}

Serial::~Serial() {
  if (fd_serial_port_ != -1) {
    ::close(fd_serial_port_);
    fd_serial_port_ = -1;
  }
}

void Serial::open(const std::string& port) {
  fd_serial_port_ = ::open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

  if (fd_serial_port_ == -1) {
    throw SerialException("Error opening port " + port + ": " + strerror(errno));
  }
  else {
    fcntl(fd_serial_port_, F_SETFL, 0);
  }
}

void Serial::close() {
  if (fd_serial_port_ != -1) {
    ssize_t error = ::close(fd_serial_port_);
    if (error < 0) {
      throw SerialException("Error closing port: " + std::string(strerror(errno)));
    }
    fd_serial_port_ = -1;
  }
}

void Serial::write(std::shared_ptr<std::string> data) {
  if (!data) {
    throw IOException("Null pointer passed to write function");
  }

  ssize_t bytes_written = ::write(fd_serial_port_, data->c_str(), data->size());

  if (bytes_written < 0) {
    throw IOException("Error writing to serial port: " + std::string(strerror(errno)));
  }
}

size_t Serial::read(std::shared_ptr<std::string> buffer) {
  if (!buffer) {
    throw IOException("Null pointer passed to read function");
  }

  if (buffer->size() > kMaxSafeReadSize) {
    throw IOException("Read buffer exceeds maximum safe limit of " +
                      std::to_string(kMaxSafeReadSize) + " bytes");
  }

  // Resize the string to accommodate the maximum possible data
  buffer->clear();
  buffer->resize(kMaxSafeReadSize);

  // Use const_cast to get non-const pointer for read operation
  ssize_t bytes_read = ::read(fd_serial_port_, const_cast<char*>(buffer->data()), kMaxSafeReadSize);
  
  if (bytes_read < 0) {
    throw IOException("Error reading from serial port: " + std::string(strerror(errno)));
  }

  std::cout << "Bytes read: " << bytes_read << std::endl;

  // Resize the string to the actual number of bytes read
  buffer->resize(static_cast<size_t>(bytes_read));

  return static_cast<size_t>(bytes_read);
}

char Serial::readByte() {
  char byte = 0;
  ssize_t bytes_read = ::read(fd_serial_port_, &byte, 1);
  if (bytes_read < 0) {
    throw SerialException("Error reading from serial port: " + std::string(strerror(errno)));
  }
  return byte;
}

size_t Serial::readUntil(std::shared_ptr<std::string> buffer, char terminator) {
  if (!buffer) {
    throw IOException("Null pointer passed to readUntil function");
  }

  buffer->clear();
  char temp_char = '\0';

  auto start_time = std::chrono::steady_clock::now();

  while (temp_char != terminator) {
    // Check buffer size limit to prevent excessive memory usage
    if (buffer->size() >= kMaxSafeReadSize) {
      throw IOException("Read buffer exceeded maximum size limit of " +
                        std::to_string(kMaxSafeReadSize) +
                        " bytes without finding terminator");
    }
    // Check timeout if enabled (0 means no timeout)
    if (read_timeout_ > 0) {
      auto current_time = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time -
                                                                           start_time).count();

      if (elapsed >= static_cast<int64_t>(read_timeout_)) {
        throw IOException("Read timeout exceeded while waiting for terminator");
      }

      // Use poll() to check if data is available with remaining timeout.
      // poll() does not have the FD_SETSIZE limitation that select() has
      // and is more robust for larger file descriptor values.
      struct pollfd pfd;
      pfd.fd = fd_serial_port_;
      pfd.events = POLLIN;

      int64_t remaining_timeout = read_timeout_ - elapsed;
      int timeout_ms = static_cast<int>(remaining_timeout);

      int poll_result = poll(&pfd, 1, timeout_ms);
      if (poll_result < 0) {
        throw IOException("Error in poll(): " + std::string(strerror(errno)));
      }
      else if (poll_result == 0) {
        throw IOException("Read timeout exceeded while waiting for data");
      }
    }

    // Data is available, perform the read
    ssize_t bytes_read = ::read(fd_serial_port_, &temp_char, 1);

    if (bytes_read < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // Non-blocking read, no data available right now
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        continue;
      }
      throw SerialException("Error reading from serial port: " + std::string(strerror(errno)));
    }
    else if (bytes_read == 0) {
      // End of file or connection closed
      throw SerialException("Connection closed while reading");
    }

    // Add the character to buffer (including terminator)
    buffer->push_back(temp_char);
  }

  return buffer->size();
}

void Serial::flushInputBuffer() {
  if (ioctl(fd_serial_port_, TCFLSH, TCIFLUSH) != 0) {
    throw SerialException("Error flushing input buffer: " + std::string(strerror(errno)));
  }
}

int Serial::getAvailableData() const {
  int bytes_available;
  if (ioctl(fd_serial_port_, FIONREAD, &bytes_available) < 0) {
    throw SerialException("Error getting available data: " + std::string(strerror(errno)));
  }
  return bytes_available;
}

void Serial::setBaudRate(int baud_rate) {
  try{
    this->getTermios2();
  options_.c_cflag &= ~CBAUD;
  options_.c_cflag |= BOTHER;
  options_.c_ispeed = baud_rate;
  options_.c_ospeed = baud_rate;
    this->setTermios2();
  }
  catch(...) {
    throw;
  }
}

void Serial::setBaudRate(BaudRate baud_rate) {
  setBaudRate(static_cast<int>(baud_rate));
}

int Serial::getBaudRate() const {
  this->getTermios2();
  return (static_cast<int>(options_.c_ispeed));
}

void Serial::getTermios2() const {
  ssize_t error = ioctl(fd_serial_port_, TCGETS2, &options_);
  if (error < 0) {
    throw SerialException("Error get Termios2: " + std::string(strerror(errno)));
  }
}

void Serial::setTermios2() {
  ssize_t error = ioctl(fd_serial_port_, TCSETS2, &options_);
  if (error < 0) {
    throw SerialException("Error set Termios2: " + std::string(strerror(errno)));
  }
}

void Serial::setReadTimeout(unsigned int timeout) {
  read_timeout_ = timeout;
}

void Serial::setWriteTimeout(unsigned int timeout) {
  write_timeout_ = timeout;
}

void Serial::setDataLength(DataLength nbits) {
  this->getTermios2();

  // Clear bits
  options_.c_cflag &= ~CSIZE;

  switch (nbits) {
  case DataLength::FIVE:
    options_.c_cflag |= CS5;
    break;
  case DataLength::SIX:
    options_.c_cflag |= CS6;
    break;
  case DataLength::SEVEN:
    options_.c_cflag |= CS7;
    break;
  case DataLength::EIGHT:
    options_.c_cflag |= CS8;
    break;
  default:
    options_.c_cflag |= CS8;
    break;
  }
  this->setTermios2();
}


void Serial::setParity([[maybe_unused]] Parity parity) {
//   this->getTermios2();
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
//   this->setTermios2();
}

void Serial::setStopBits([[maybe_unused]] StopBits stop_bits) {
//   this->getTermios2();
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
//   this->setTermios2();
}

void Serial::setFlowControl([[maybe_unused]] FlowControl flow_control) {
//   this->getTermios2();
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
//   this->setTermios2();
}

void Serial::setCanonicalMode([[maybe_unused]] CanonicalMode canonical_mode) {
//   this->getTermios2();
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
//   this->setTermios2();
}

void Serial::setTerminator([[maybe_unused]] Terminator term) {
//   terminator_ = (int)term;
}

void Serial::setTimeOut([[maybe_unused]] int time) {
//   this->getTermios2();
//   options_.c_cc[VTIME] = time;
//   this->setTermios2();
}

void Serial::setMinNumberCharRead([[maybe_unused]] int num) {
//   this->getTermios2();
//   options_.c_cc[VMIN] = num;
//   this->setTermios2();
}
}  // namespace libserial
