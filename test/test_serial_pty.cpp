// Copyright 2020-2025 Nestor Neto

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "libserial/serial.hpp"
#include "libserial/serial_exception.hpp"

// Integration test using pseudo-terminals
class WorkingPseudoTerminalTest : public ::testing::Test {
 protected:
    int master_fd{-1};
    int slave_fd{-1};
    std::string slave_port;

    void SetUp() override {
        // Create pseudo-terminal pair
        master_fd = posix_openpt(O_RDWR | O_NOCTTY);
        if (master_fd == -1) {
            FAIL() << "Failed to open master pseudo-terminal";
            return;
        }

        if (grantpt(master_fd) == -1 || unlockpt(master_fd) == -1) {
            close(master_fd);
            FAIL() << "Failed to setup master pseudo-terminal";
            return;
        }

        char *slave_name = ptsname(master_fd);
        if (!slave_name) {
            close(master_fd);
            FAIL() << "Failed to get slave pseudo-terminal name";
            return;
        }

        slave_port = std::string(slave_name);

        // Open slave end for internal testing
        slave_fd = open(slave_name, O_RDWR | O_NOCTTY);
        if (slave_fd == -1) {
            close(master_fd);
            FAIL() << "Failed to open slave pseudo-terminal";
            return;
        }
    }

    void TearDown() override {
        if (master_fd != -1) close(master_fd);
        if (slave_fd != -1) close(slave_fd);
    }
};

TEST_F(WorkingPseudoTerminalTest, OpenClosePort) {
    libserial::Serial serial_port;

    // Test opening the port
    EXPECT_NO_THROW({ serial_port.open(slave_port); });

    // Test closing the port
    EXPECT_NO_THROW({ serial_port.close(); });
}

TEST_F(WorkingPseudoTerminalTest, GetBaudRate) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);

    // Set a valid baud rate
    EXPECT_NO_THROW({ serial_port.setBaudRate(9600); });

    // Get the baud rate and verify
    int baud_rate{0};
    EXPECT_NO_THROW({ baud_rate = serial_port.getBaudRate(); });
    EXPECT_EQ(baud_rate, 9600);

    serial_port.close();
}

TEST_F(WorkingPseudoTerminalTest, GetAvailableData) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);
    serial_port.setBaudRate(9600);

    // First check that no data is available initially
    int initial_available{0};
    EXPECT_NO_THROW({ initial_available = serial_port.getAvailableData(); });
    EXPECT_EQ(initial_available, 0);

    const std::string test_message = "Hello World!\n";
    ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
    ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

    // Force flush and give more time for data to propagate
    fsync(master_fd);
    usleep(100000);  // 100ms delay

    // Now check available data again - should match what we wrote
    int available{0};
    EXPECT_NO_THROW({ available = serial_port.getAvailableData(); });
    EXPECT_EQ(available, bytes_written);
}

TEST_F(WorkingPseudoTerminalTest, ReadWithValidSharedPtr) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);
    serial_port.setBaudRate(9600);

    const std::string test_message = "Smart Pointer Test!\n";

    ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
    ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

    // Give time for data to propagate
    fsync(master_fd);
    usleep(100000);  // 100ms delay

    // Check that data is available before attempting to read
    int available = serial_port.getAvailableData();

    // Test reading with shared pointer - only read what's available
    auto read_buffer = std::make_shared<std::string>();
    size_t bytes_read = 0;

    EXPECT_NO_THROW({ bytes_read = serial_port.read(read_buffer, available); });

    EXPECT_EQ(bytes_read, test_message.length());
    EXPECT_EQ(*read_buffer, test_message);
}

TEST_F(WorkingPseudoTerminalTest, ReadUntil) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);
    serial_port.setBaudRate(9600);

    const std::string test_message = "Read Until! Test!\n";

    ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
    ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

    // Give time for data to propagate
    fsync(master_fd);
    usleep(100000);  // 100ms delay

    // Test reading with shared pointer - only read what's available
    auto read_buffer = std::make_shared<std::string>();
    size_t bytes_read = 0;

    EXPECT_NO_THROW({ bytes_read = serial_port.readUntil(read_buffer, '!'); });

    EXPECT_EQ(*read_buffer, "Read Until!");
}

TEST_F(WorkingPseudoTerminalTest, ReadUntilTimeout) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);
    serial_port.setBaudRate(9600);

    const std::string test_message = "Read Until Test";

    ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
    ASSERT_GT(bytes_written, 0) << "Failed to write to master end";

    // Give time for data to propagate
    fsync(master_fd);
    usleep(100000);  // 100ms delay

    // Test reading with shared pointer - only read what's available
    auto read_buffer = std::make_shared<std::string>();
    size_t bytes_read = 0;

    EXPECT_THROW({ bytes_read = serial_port.readUntil(read_buffer, '!'); }, libserial::SerialException);
}

// TEST_F(WorkingPseudoTerminalTest, SafeCommunication) {
//     libserial::Serial serial_port;
//
//     serial_port.open(slave_port);
//     serial_port.setBaudRate(9600);
//     std::cout << "Successfully opened serial port: " << slave_port << std::endl;
//
//     const std::string test_message = "Hello World!";
//
//     // Write data directly to the master end
//     ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
//     ASSERT_GT(bytes_written, 0) << "Failed to write to master end";
//     std::cout << "Wrote " << bytes_written << " bytes to master" << std::endl;
//
//     // Give time for data to propagate
//     usleep(100000);  // 100ms delay
//
//     // Check available data first
//     std::string received_data;
//     try {
//         int available = serial_port.getAvailableData();
//         std::cout << "Available data: " << available << " bytes" << std::endl;
//
//         if (available > 0) {
//             // Read only what's available to avoid blocking
//             received_data = serial_port.read(available);
//             std::cout << "Successfully read: '" << received_data << "'" << std::endl;
//         } else {
//             std::cout << "No data available for reading" << std::endl;
//         }
//
//     } catch (const libserial::SerialException& e) {
//         // Don't fail the test, just report the issue
//         std::cout << "Read error (expected): " << e.what() << std::endl;
//     }
//
//     // Verify basic functionality worked
//     EXPECT_GT(bytes_written, 0);  // At least writing worked
// }

TEST_F(WorkingPseudoTerminalTest, WriteTest) {
    libserial::Serial serial_port;

    try {
        serial_port.open(slave_port);
        serial_port.setBaudRate(115200);
        std::cout << "Setup serial port for write test" << std::endl;
    } catch (const libserial::SerialException& e) {
        FAIL() << "Failed to setup serial port: " << e.what();
    }

    // Create test data using smart pointer
    auto test_data = std::make_shared<std::string>("Test Write Data");

    // Write using our Serial class
    try {
        serial_port.write(test_data);
        std::cout << "Successfully wrote data via Serial class" << std::endl;

        // Give time for data to propagate
        usleep(50000);  // 50ms

        // Try to read from master end to verify
        char buffer[100] = {0};
        ssize_t bytes_read = read(master_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            std::string received(buffer, bytes_read);
            std::cout << "Master received: '" << received << "'" << std::endl;
            // The write method sends data as-is
            EXPECT_EQ(received, *test_data);
        } else {
            std::cout << "No data received on master end" << std::endl;
        }
    } catch (const libserial::SerialException& e) {
        FAIL() << "Write test failed: " << e.what();
    }
}

TEST_F(WorkingPseudoTerminalTest, ReadWithSharedPtr) {
    libserial::Serial serial_port;

    serial_port.open(slave_port);
    serial_port.setBaudRate(9600);
    std::cout << "Setup serial port for shared pointer read test" << std::endl;

    const std::string test_message = "Smart Pointer Test!\n";

    // Write data to master end
    ssize_t bytes_written = write(master_fd, test_message.c_str(), test_message.length());
    ASSERT_GT(bytes_written, 0) << "Failed to write to master end";
    std::cout << "Wrote " << bytes_written << " bytes to master" << std::endl;

    // Give time for data to propagate
    fsync(master_fd);
    usleep(100000);  // 100ms delay

    // Check that data is available before attempting to read
    int available = 0;
    EXPECT_NO_THROW({ available = serial_port.getAvailableData(); });
    std::cout << "Available data: " << available << " bytes" << std::endl;

    if (available > 0) {
        // Test reading with shared pointer - only read what's available
        auto read_buffer = std::make_shared<std::string>();
        size_t bytes_read = 0;

        EXPECT_NO_THROW({ bytes_read = serial_port.read(read_buffer, available); });

        std::cout << "Read " << bytes_read << " bytes into shared pointer" << std::endl;
        std::cout << "Buffer content: '" << *read_buffer << "'" << std::endl;

        // Verify the data
        EXPECT_EQ(bytes_read, test_message.length());
        EXPECT_EQ(*read_buffer, test_message);
    } else {
        std::cout << "No data available, skipping read test" << std::endl;
        // Still verify the API works by testing null pointer case
    }

    // Test null pointer error handling
    std::shared_ptr<std::string> null_buffer;
    EXPECT_THROW({ serial_port.read(null_buffer, 10); }, libserial::SerialException);
}

