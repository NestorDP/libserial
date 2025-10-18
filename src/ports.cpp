// @ Copyright 2020-2025 Nestor Neto

/**
 *
 *
 * \file serial.cpp
 */
#include <cstdio>
#include <string>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <utility>

#include "libserial/ports.hpp"

namespace libserial {

uint16_t Ports::scanPorts() {
  devices_.clear();

  // Directory where udev creates symlinks for serial devices by ID
  // this directory may not exist if no serial devices are connected
  const char* by_id_dir = kSysSerialByIdPath;
  DIR* dir = opendir(by_id_dir);
  if (!dir) {
    std::cout << "No serial devices directory: " << by_id_dir << "\n";
    return -1;
  }

  // The POSIX directory-entry structure used by readdir() to describe files
  // inside a directory.
  struct dirent* entry;
  uint16_t id_counter = 0;

  while ((entry = readdir(dir)) != nullptr) {
    // Skip . and .. entries
    if (entry->d_name[0] == '.') continue;

    std::string device_name(entry->d_name);
    std::string symlink_path = std::string(by_id_dir) + "/" + device_name;

    // Store the relative path the symlink points to
    char target[PATH_MAX] = {0};

    // Resolve the symlink to get the actual device path relative to /dev
    // from the /dev/serial/by-id/ directory (e.g., ../../ttyUSB0)
    ssize_t len = readlink(symlink_path.c_str(), target, sizeof(target) - 1);
    target[len] = '\0';

    // Resolve the relative path to an absolute path
    const char* bname = strrchr(target, '/');
    bname++;

    // Construct the full /dev/ttyXXX path
    auto resolved = std::string("/dev/") + std::string(bname);

    // Add to device list
    Device dev;
    dev.setId(id_counter++);
    dev.setName(device_name);
    dev.setBusPath(resolved);
    dev.setPortPath(resolved);

    // Update the device list
    devices_.push_back(std::move(dev));
  }

  closedir(dir);

  return (id_counter - 1);
}

void Ports::getDevices(std::vector<Device> & devices) const {
  devices = devices_;
}

std::optional<std::string> Ports::findPortPath(uint16_t id) const {
  auto it = std::find_if(devices_.cbegin(), devices_.cend(),
                         [id](const Device& dev){
      return dev.getId() == id;
    });
  if (it == devices_.cend()) return std::nullopt;
  return it->getPortPath();
}

std::optional<std::string> Ports::findBusPath(uint16_t id) const {
  auto it = std::find_if(devices_.cbegin(), devices_.cend(),
                         [id](const Device& dev){
      return dev.getId() == id;
    });
  if (it == devices_.cend()) return std::nullopt;
  return it->getBusPath();
}

std::optional<std::string> Ports::findName(uint16_t id) const {
  auto it = std::find_if(devices_.cbegin(), devices_.cend(),
                         [id](const Device& dev){
      return dev.getId() == id;
    });
  if (it == devices_.cend()) return std::nullopt;
  return it->getName();
}

}  // namespace libserial
