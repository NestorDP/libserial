// Copyright 2020-2025 Nestor Neto

#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <iostream>

#include "libserial/ports.hpp"
#include "libserial/serial_exception.hpp"

TEST(DeviceTest, DefaultConstructor) {
  EXPECT_NO_THROW({
    libserial::Device device;
  });
}

TEST(DeviceTest, ParameterizedConstructor) {
  libserial::Device device("TestDevice",
                           "/dev/ttyUSB0",
                           "/dev/bus/usb/001/002",
                           42);

  EXPECT_EQ(device.getName(), "TestDevice");
  EXPECT_EQ(device.getPortPath(), "/dev/ttyUSB0");
  EXPECT_EQ(device.getBusPath(), "/dev/bus/usb/001/002");
  EXPECT_EQ(device.getId(), 42);
}

TEST(DeviceTest, SetGetFunction) {
  libserial::Device device;

  device.setName("NewDevice");
  device.setPortPath("/dev/ttyS1");
  device.setBusPath("/dev/bus/usb/003/004");
  device.setId(7);

  EXPECT_EQ(device.getName(), "NewDevice");
  EXPECT_EQ(device.getPortPath(), "/dev/ttyS1");
  EXPECT_EQ(device.getBusPath(), "/dev/bus/usb/003/004");
  EXPECT_EQ(device.getId(), 7);
}


