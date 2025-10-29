// Copyright 2020-2025 Nestor Neto

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "libserial/serial.hpp"
#include "libserial/serial_exception.hpp"

// Integration test using pseudo-terminals
class PseudoTerminalTest : public ::testing::Test {
protected:
int master_fd_{-1};
int slave_fd_{-1};
std::string slave_port_;

void SetUp() override {
  // Create pseudo-terminal pair
  master_fd_ = posix_openpt(O_RDWR | O_NOCTTY);
  if (master_fd_ == -1) {
    FAIL() << "Failed to open master pseudo-terminal";
    return;
  }

  if (grantpt(master_fd_) == -1 || unlockpt(master_fd_) == -1) {
    close(master_fd_);
    FAIL() << "Failed to setup master pseudo-terminal";
    return;
  }

  char *slave_name = ptsname(master_fd_);
  if (!slave_name) {
    close(master_fd_);
    FAIL() << "Failed to get slave pseudo-terminal name";
    return;
  }

  slave_port_ = std::string(slave_name);

  // Open slave end for internal testing
  slave_fd_ = open(slave_name, O_RDWR | O_NOCTTY);
  if (slave_fd_ == -1) {
    close(master_fd_);
    FAIL() << "Failed to open slave pseudo-terminal";
    return;
  }

  errors_poll_ = {
    {EAGAIN, "Resource temporarily unavailable"},
    {ENOMEM, "Cannot allocate memory"},
    {EINVAL, "Invalid argument"},
    {EPERM, "Operation not permitted"},
    {EBADF, "Bad file descriptor"},
    {EEXIST, "File exists"},
    {ENOENT, "No such file or directory"},
    {EINTR, "Interrupted system call"}
  };

  errors_read_ = {
    {EBADF, "Bad file descriptor"},
    {EIO, "Input/output error"},
    {EINTR, "Interrupted system call"},
    {EAGAIN, "Resource temporarily unavailable"},
    {EWOULDBLOCK, "Resource temporarily unavailable"},
    {EISDIR, "Is a directory"}
  };
}

void TearDown() override {
  if (master_fd_ != -1) close(master_fd_);
  if (slave_fd_ != -1) close(slave_fd_);
}

std::vector<std::pair<int, std::string> > errors_poll_;
std::vector<std::pair<int, std::string> > errors_read_;
};

TEST_F(PseudoTerminalTest, OpenClosePort) {
  libserial::Serial serial_port;

  // Test opening the port
  EXPECT_NO_THROW({ serial_port.open(slave_port_); });

  // Test closing the port
  EXPECT_NO_THROW({ serial_port.close(); });
}

TEST_F(PseudoTerminalTest, ParameterizedConstructor) {
  libserial::Serial serial_port(slave_port_);
}

// TEST_F(PseudoTerminalTest, SetTermios2WithFail) {
//   libserial::Serial serial_port;

//   serial_port.open(slave_port_);

//   // Inject failure into ioctl for setTermios2
//   serial_port.setIoctlSystemFunction(
//     [](int, unsigned long, void*) -> int {  // NOLINT
//     errno = EIO;
//     return -1;
//   });

//   EXPECT_THROW({
//     serial_port.setBaudRate(9600);
//   }, libserial::SerialException);

//   // Restore ioctl function for cleanup
//   serial_port.setIoctlSystemFunction(
//     [](int fd, unsigned long request, void* arg) -> int {  // NOLINT
//     return ::ioctl(fd, request, arg);
//   });

//   serial_port.close();
// }

TEST_F(PseudoTerminalTest, SetAndGetBaudRate) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);

  // Set baud rate using int
  EXPECT_NO_THROW({ serial_port.setBaudRate(9600); });

  // Get baud rate and verify
  int baud_rate = 0;
  EXPECT_NO_THROW({ baud_rate = serial_port.getBaudRate(); });
  EXPECT_EQ(baud_rate, 9600);

  // Set baud rate using BaudRate enum
  EXPECT_NO_THROW({ serial_port.setBaudRate(libserial::BaudRate::BAUD_RATE_115200); });

  // Get the baud rate and verify
  EXPECT_NO_THROW({ baud_rate = serial_port.getBaudRate(); });
  EXPECT_EQ(baud_rate, 115200);

  serial_port.close();
}

// TEST_F(PseudoTerminalTest, SetAndGetDataLength) {
//   libserial::Serial serial_port;

//   serial_port.open(slave_port_);

//   // Test multiple data lengths to be more thorough
//   std::vector<libserial::DataLength> test_lengths = {
//     libserial::DataLength::FIVE,
//     libserial::DataLength::SIX,
//     libserial::DataLength::SEVEN,
//     libserial::DataLength::EIGHT
//   };

//   for (const auto& expected_length : test_lengths) {
//     // Set data length
//     EXPECT_NO_THROW({
//       serial_port.setDataLength(expected_length);
//     });

//     // Add a small delay and flush
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));

//     // Force a re-read of the current settings
//     serial_port.close();
//     serial_port.open(slave_port_);

//     // Get data length and verify
//     libserial::DataLength actual_length;
//     EXPECT_NO_THROW({
//       actual_length = serial_port.getDataLength();
//     });

//     EXPECT_EQ(actual_length, expected_length)
//       << "Failed for data length: " << static_cast<int>(expected_length);
//   }

//   serial_port.close();
// }

TEST_F(PseudoTerminalTest, SetGetReadTimeout) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);

  // Set read timeout
  std::chrono::milliseconds timeout_set{1500};
  EXPECT_NO_THROW({ serial_port.setReadTimeout(timeout_set); });

  // Get read timeout and verify
  std::chrono::milliseconds timeout_get{0};
  EXPECT_NO_THROW({ timeout_get = serial_port.getReadTimeout(); });
  EXPECT_EQ(timeout_get.count(), timeout_set.count());

  serial_port.close();
}

TEST_F(PseudoTerminalTest, SetParity) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);

  // Set parity to ENABLE
  EXPECT_NO_THROW({ serial_port.setParity(libserial::Parity::ENABLE); });

  // Set parity to ODD
  EXPECT_NO_THROW({ serial_port.setParity(libserial::Parity::DISABLE); });

  serial_port.close();
}

TEST_F(PseudoTerminalTest, SetStopBits) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);

  // Set stop bits to 1
  EXPECT_NO_THROW({ serial_port.setStopBits(libserial::StopBits::ONE); });

  // Set stop bits to 2
  EXPECT_NO_THROW({ serial_port.setStopBits(libserial::StopBits::TWO); });

  serial_port.close();
}

TEST_F(PseudoTerminalTest, GetAvailableData) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  // First check that no data is available initially
  int initial_available{0};
  EXPECT_NO_THROW({ initial_available = serial_port.getAvailableData(); });
  EXPECT_EQ(initial_available, 0);

  const std::string test_message = "Hello World!\n";
  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Force flush and give more time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Now check available data again - should match what we wrote
  int available{0};
  EXPECT_NO_THROW({ available = serial_port.getAvailableData(); });
  EXPECT_EQ(available, bytes_written);
}

TEST_F(PseudoTerminalTest, WriteTest) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(115200);

  // Create test data using smart pointer
  auto test_data = std::make_shared<std::string>("Test Write Data");

  // Write using our Serial class
  EXPECT_NO_THROW({ serial_port.write(test_data); });

  // Give time for data to propagate
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  // Try to read from master end to verify
  char buffer[100] = {0};
  ssize_t bytes_read = read(master_fd_, buffer, sizeof(buffer) - 1);

  std::string received(buffer, bytes_read);

  EXPECT_EQ(received, *test_data);
}

TEST_F(PseudoTerminalTest, ReadCanonicalMode) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  const std::string test_message{"Smart Pointer Test!\n"};

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test reading with shared pointer
  auto read_buffer = std::make_shared<std::string>();
  size_t bytes_read = 0;

  EXPECT_NO_THROW({ bytes_read = serial_port.read(read_buffer); });

  EXPECT_EQ(bytes_read, test_message.length());
  EXPECT_EQ(*read_buffer, test_message);
}

TEST_F(PseudoTerminalTest, ReadWithNullBuffer) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  std::shared_ptr<std::string> null_buffer;

  EXPECT_THROW({
    try {
      serial_port.read(null_buffer);
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ("Null pointer passed to read function", e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadNonCanonicalMode) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::DISABLE);

  const std::string test_message{"Non-Canonical Test\n"};

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Attempt to read using read() - should throw exception
  auto read_buffer = std::make_shared<std::string>();

  EXPECT_THROW({
    try {
      serial_port.read(read_buffer);
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ(
        "read() is not supported in non-canonical mode; use readBytes() or readUntil() instead",
        e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadTimeout) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  // Set a short read timeout
  int time_out_ms = 100;
  serial_port.setReadTimeout(std::chrono::milliseconds(time_out_ms));

  auto read_buffer = std::make_shared<std::string>();

  auto expected_what = "Read operation timed out after " + std::to_string(time_out_ms) +
                       " milliseconds";

  EXPECT_THROW({
    try {
      serial_port.read(read_buffer);
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ(expected_what.c_str(), e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadWithReadFail) {
  libserial::Serial serial_port;
  auto read_buffer = std::make_shared<std::string>();

  for (const auto& [error_num, error_msg] : errors_read_) {
    serial_port.setPollSystemFunction(
      [](struct pollfd*, nfds_t, int) -> int {
      return 1;
    });
    serial_port.setReadSystemFunction(
      [error_num](int, void*, size_t) -> ssize_t {
      errno = error_num;
      return -1;
    });

    auto expected_what = "Error reading from serial port: " + error_msg;

    EXPECT_THROW({
      try {
        serial_port.read(read_buffer);
      }
      catch (const libserial::IOException& e) {
        EXPECT_STREQ(expected_what.c_str(), e.what());
        throw;
      }
    }, libserial::IOException);
  }
}

TEST_F(PseudoTerminalTest, ReadWithPollFail) {
  libserial::Serial serial_port;
  auto read_buffer = std::make_shared<std::string>();

  for (const auto& [error_num, error_msg] : errors_poll_) {
    serial_port.setPollSystemFunction(
      [error_num](struct pollfd*, nfds_t, int) -> int {
      errno = error_num;
      return -1;
    });

    auto expected_what = "Error in poll(): " + error_msg;

    EXPECT_THROW({
      try {
        serial_port.read(read_buffer);
      }
      catch (const libserial::IOException& e) {
        EXPECT_STREQ(expected_what.c_str(), e.what());
        throw;
      }
    }, libserial::IOException);
  }
}

TEST_F(PseudoTerminalTest, ReadBytesNonCanonicalMode) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::DISABLE);

  const std::string test_message{"ReadBytes Test!"};

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test reading with shared pointer
  auto read_buffer = std::make_shared<std::string>();
  size_t bytes_read = 0;

  EXPECT_NO_THROW({ bytes_read = serial_port.readBytes(read_buffer, test_message.length()); });

  EXPECT_EQ(bytes_read, test_message.length());
  EXPECT_EQ(*read_buffer, test_message);
}

TEST_F(PseudoTerminalTest, ReadBytesWithNullBuffer) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::DISABLE);

  std::shared_ptr<std::string> null_buffer;

  EXPECT_THROW({
    try {
      serial_port.readBytes(null_buffer, 10);
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ("Null pointer passed to readBytes function", e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadBytesWithInvalidNumBytes) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::DISABLE);

  auto read_buffer = std::make_shared<std::string>();

  EXPECT_THROW({
    try {
      serial_port.readBytes(read_buffer, 0);
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ("Number of bytes requested must be greater than zero", e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadBytesWithReadFail) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::DISABLE);

  auto read_buffer = std::make_shared<std::string>();

  for (const auto& [error_num, error_msg] : errors_read_) {
    serial_port.setReadSystemFunction(
      [error_num](int, void*, size_t) -> ssize_t {
      errno = error_num;
      return -1;
    });

    auto expected_what = "Error reading from serial port: " + error_msg;

    EXPECT_THROW({
      try {
        serial_port.readBytes(read_buffer, 10);
      }
      catch (const libserial::IOException& e) {
        EXPECT_STREQ(expected_what.c_str(), e.what());
        throw;
      }
    }, libserial::IOException);
  }
}

TEST_F(PseudoTerminalTest, ReadBytesCanonicalMode) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  serial_port.setCanonicalMode(libserial::CanonicalMode::ENABLE);

  auto read_buffer = std::make_shared<std::string>();

  EXPECT_THROW({
    try {
      serial_port.readBytes(read_buffer, 5);
      ADD_FAILURE() << "Expected SerialException but no exception was thrown";
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ(
        "readBytes() is not supported in canonical mode; use read() or readUntil() instead",
        e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadUntil) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  const std::string test_message = "Read Until! Test!\n";

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test reading with shared pointer - only read what's available
  auto read_buffer = std::make_shared<std::string>();

  EXPECT_NO_THROW({serial_port.readUntil(read_buffer, '!'); });

  EXPECT_EQ(*read_buffer, "Read Until!");
}

TEST_F(PseudoTerminalTest, ReadUntilWithNullBuffer) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  std::shared_ptr<std::string> null_buffer;

  EXPECT_THROW({
    try {
      serial_port.readUntil(null_buffer, '!');
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ("Null pointer passed to readUntil function", e.what());
      throw;
    }
  }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadUntilTimeout) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);

  const std::string test_message = "Read Until Test";

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test reading with shared pointer - only read what's available
  auto read_buffer = std::make_shared<std::string>();

  EXPECT_THROW({serial_port.readUntil(read_buffer, '!'); }, libserial::IOException);
}

TEST_F(PseudoTerminalTest, ReadUntilWithReadFail) {
  libserial::Serial serial_port;
  auto read_buffer = std::make_shared<std::string>();

  for (const auto& [error_num, error_msg] : errors_read_) {
    if (error_num == EAGAIN || error_num == EWOULDBLOCK) {
      // Skip these as they are handled differently in readUntil
      continue;
    }
    serial_port.setPollSystemFunction(
      [](struct pollfd*, nfds_t, int) -> int {
      return 1;
    });
    serial_port.setReadSystemFunction(
      [error_num](int, void*, size_t) -> ssize_t {
      errno = error_num;
      return -1;
    });

    auto expected_what = "Error reading from serial port: " + error_msg;

    EXPECT_THROW({
      try {
        serial_port.readUntil(read_buffer, '!');
      }
      catch (const libserial::IOException& e) {
        EXPECT_STREQ(expected_what.c_str(), e.what());
        throw;
      }
    }, libserial::IOException);
  }
}

TEST_F(PseudoTerminalTest, ReadUntilWithPollFail) {
  libserial::Serial serial_port;
  auto read_buffer = std::make_shared<std::string>();

  for (const auto& [error_num, error_msg] : errors_poll_) {
    serial_port.setPollSystemFunction(
      [error_num](struct pollfd*, nfds_t, int) -> int {
      errno = error_num;
      return -1;
    });

    auto expected_what = "Error in poll(): " + error_msg;

    EXPECT_THROW({
      try {
        serial_port.readUntil(read_buffer, '!');
      }
      catch (const libserial::IOException& e) {
        EXPECT_STREQ(expected_what.c_str(), e.what());
        throw;
      }
    }, libserial::IOException);
  }
}

TEST_F(PseudoTerminalTest, ReadUntilWithOverflowBuffer) {
  libserial::Serial serial_port;

  serial_port.open(slave_port_);
  serial_port.setBaudRate(9600);
  EXPECT_NO_THROW(serial_port.setMaxSafeReadSize(10));  // Set max safe read size to 10 bytes

  std::string test_message(15, 'a');
  test_message.push_back('\n');

  ssize_t bytes_written = write(master_fd_, test_message.c_str(), test_message.length());
  ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

  // Give time for data to propagate
  fsync(master_fd_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test reading with shared pointer - only read what's available
  auto read_buffer = std::make_shared<std::string>();

  auto expected_what = "Read buffer exceeded maximum size limit of " +
                       std::to_string(serial_port.getMaxSafeReadSize()) +
                       " bytes without finding terminator";

  EXPECT_THROW({
    try {
      serial_port.readUntil(read_buffer, '!');
    }
    catch (const libserial::IOException& e) {
      EXPECT_STREQ(expected_what.c_str(), e.what());
      throw;
    }
  }, libserial::IOException);
}
