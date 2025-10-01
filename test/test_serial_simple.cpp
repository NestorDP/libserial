#include <gtest/gtest.h>
#include <memory>
#include <string>

// Include libserial headers
#include "libserial/serial.hpp"
#include "libserial/serial_exception.hpp"

// Simple unit tests that don't require actual hardware
class SerialTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test setup if needed
    }

    void TearDown() override {
        // Test cleanup if needed
    }
};

TEST_F(SerialTest, DefaultConstructor) {
    EXPECT_NO_THROW({
        libserial::Serial serial;
    });
}

TEST_F(SerialTest, ConstructorWithInvalidPort) {
    EXPECT_THROW({
        libserial::Serial serial("/dev/nonexistent");
    }, libserial::SerialException);
}

TEST_F(SerialTest, WriteWithSharedPtr) {
    libserial::Serial serial;
    
    // Test that write function accepts shared_ptr
    auto message = std::make_shared<std::string>("Test message");
    
    // This will throw since no port is opened, but tests the API
    EXPECT_THROW({
        serial.write(message);
    }, libserial::SerialException);
}

TEST_F(SerialTest, WriteWithNullPtr) {
    libserial::Serial serial;
    
    // Test that write function handles null pointer
    std::shared_ptr<std::string> null_message;
    
    EXPECT_THROW({
        serial.write(null_message);
    }, libserial::SerialException);
}

TEST_F(SerialTest, APIExists) {
    libserial::Serial serial;
    
    // close() should not throw when no port is open (safe operation)
    EXPECT_NO_THROW(serial.close());
    
    // These should all throw exceptions since no port is open,
    // but they test that the API methods exist and are callable
    EXPECT_THROW(serial.read(100), libserial::SerialException);
    EXPECT_THROW(serial.readUntil('\n'), libserial::SerialException);
    EXPECT_THROW(serial.flushInputBuffer(), libserial::SerialException);
    EXPECT_THROW(serial.setBaudRate(9600), libserial::SerialException);
    EXPECT_THROW(serial.getAvailableData(), libserial::SerialException);
}