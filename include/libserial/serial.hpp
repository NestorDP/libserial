//  @ Copyright 2022 Nestor Neto

/**
 * @mainpage Serial Interface Library
 * @section intro_sec Indroduction
 * This is an interface C++ library for serial serial
 * 
 * 
 */
#ifndef LIBSERIAL_SERIAL_HPP_
#define LIBSERIAL_SERIAL_HPP_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <sys/ioctl.h>

#include <iostream>
#include <memory>
#include <string>

/**
 * @brief Ethernet Interface Library SERIAL namespace
 * @author Nestor Pereira Neto
 * 
 */

namespace serial {
  /**
   * @brief Enum class
   */
  enum class NumBits {
    Five,
    Six,
    Seven,
    Eight,
  };

  /**
   * @brief Enum class
   */
  enum class Parity {
    Enable,
    Disable,
  };

  enum class StopBits {
    Enable,
    Disable,
  };

  enum class FlowControl {
    Hardware,
    Software,
  };

  enum class CanonicalMode {
    Enable,
    Disable,
  };

  enum class Terminator {
    EOT = 4,
    CR  = 13,
  };

class Serial {
 public:
  /**
   * @brief Constructor of the serial class
   */
  Serial();

  /**
   * @brief Destroyer of the serial class
   * 
   */
  ~Serial();

    /**
   * @brief Configure the number of bits per byte. 
   * 
   * @param int numBits
   * @return void
   */
  void GetTermios2();

  /**
   * @brief Configure the number of bits per byte. 
   * 
   * @param int numBits
   * @return void
   */
  void SetTermios2();
  
  /**
   * @brief Method to open the serial communication
   * 
   */
  void OpenPort(std::string port);

  /**
   * @brief Send string over serial port
   * 
   * @param std::shared_ptr<std::string> msg_ptr
   */
  void SendMsg(std::shared_ptr<std::string> msgPtr);

  /**
   * @brief Receive string over serial port
   * 
   * @param N/D
   * @return void 
   */
  void ReceiveMsg(std::string* msgPtr);

  /**
   * @brief Configure the number of bits per byte. 
   * 
   * @param  num_bits
   * @return void
   */
  void SetNumberBits(NumBits mum_bits);

    /**
   * @brief Configure the parity. 
   * 
   * @param int numBits
   * @return void
   */
  void SetParity(Parity parity);

  void SetTwoStopBits(StopBits stop_bits);

  void SetFlowControl(FlowControl flow_control);

  void SetCanonicalMode(CanonicalMode canonical_mode);

  void SetTerminator(Terminator term);

  void SetTimeOut(int time);

  void SetMinNumberCharRead(int num);


 private:
  /** Termios struct needed to access serial communication
   *  configuration parameters */
  struct termios2 options_;

  /** File descriptor for the port */
  int fd_serial_port_;

  /** Length of buffer */
  int kLengthBuffer_ = 200;

  /**  */
  int error_;

  int terminator_;
};

}  // namespace serial

#endif  // LIBSERIAL_SERIAL_HPP_
