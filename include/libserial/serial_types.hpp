#ifndef SERIAL_TYPES_HPP
#define SERIAL_TYPES_HPP

namespace serial {

  enum class Parity {
    ENABLE,
    DISABLE,
  };

  enum class StopBits {
    ENABLE,
    DISABLE,
  };

  enum class FlowControl {
    HARDWARE,
    SOFTWARE,
  };

  enum class CanonicalMode {
    ENABLE,
    DISABLE,
  };

  enum class Terminator {
    EOT = 4,
    CR  = 13,
  };

  enum class BaudRate {
    BAUD_RATE_50 = 50,
    BAUD_RATE_75 = 75,
    BAUD_RATE_110 = 110,
    BAUD_RATE_134 = 134,
    BAUD_RATE_150 = 150,
    BAUD_RATE_200 = 200,
    BAUD_RATE_300 = 300,
    BAUD_RATE_600 = 600,
    BAUD_RATE_1200 = 1200,
    BAUD_RATE_1800 = 1800,
    BAUD_RATE_2400 = 2400,
    BAUD_RATE_4800 = 4800,
    BAUD_RATE_9600 = 9600,
    BAUD_RATE_19200 = 19200,
    BAUD_RATE_38400 = 38400,
    BAUD_RATE_57600 = 57600,
    BAUD_RATE_115200 = 115200,
    BAUD_RATE_230400 = 230400
  };

}  // namespace serial

#endif // SERIAL_TYPES_HPP