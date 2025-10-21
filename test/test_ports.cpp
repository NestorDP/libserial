// Copyright 2020-2025 Nestor Neto

#include <gtest/gtest.h>
#include <memory>
#include <string>

// Include libserial headers
#include "libserial/ports.hpp"
#include "libserial/serial_exception.hpp"

// Simple unit tests that don't require actual hardware
class PortsTest : public ::testing::Test {
protected:
void SetUp() override {
  // Test setup if needed
}

void TearDown() override {
  // Test cleanup if needed
}
};

TEST_F(PortsTest, DefaultConstructor) {
  EXPECT_NO_THROW({
    libserial::Ports ports;
  });
}

TEST_F(PortsTest, ScanPortsThrowsWhenPathMissing) {
  // Use an unlikely path to exist to trigger the exception path
  const char* missing_path = "/this/path/should/not/exist/serial/by-id";
  libserial::Ports ports(missing_path);


  try {
    (void)ports.scanPorts();
  } catch (const libserial::SerialException& e) {
    std::cout << "Caught SerialException: " << e.what() << std::endl;
    // Optionally assert something about the message:
    // EXPECT_NE(std::string(e.what()).find("Error while reading"), std::string::npos);
  }

  EXPECT_THROW({
    (void)ports.scanPorts();
  }, libserial::PortNotFoundException);
}


