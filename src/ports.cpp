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
  const char* by_id_dir = sys_path_;
  DIR* dir = opendir(by_id_dir);
  if (!dir) {
    throw PortNotFoundException("Error while reading " + std::string(by_id_dir) + ": " +
                                strerror(errno));
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
    std::string target;
    try {
      getSymlinkTarget(symlink_path, target);
    } catch (const ScanPortsException& e) {
      throw ScanPortsException("Failed to get symlink target for " + symlink_path + ": " + e.what());
      continue;
    }
    
    // Resolve the relative path to an absolute path
    const char* bname = strrchr(target.c_str(), '/');
    if (!bname) {
      // No slash found - malformed symlink, skip
      std::cerr << "Warning: Malformed symlink target for " << symlink_path << std::endl;
      continue;
    }
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

  return id_counter;
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

void Ports::getSymlinkTarget(const std::string& symlink_path, std::string& target) {
  struct stat stat_buf;
  char buffer[PATH_MAX];
  
  // Verify it's actually a symlink
  if (lstat(symlink_path.c_str(), &stat_buf) == -1) {
      throw ScanPortsException("lstat failed for " + symlink_path + ": " +
                               std::string(strerror(errno)));
  }
  
  if (!S_ISLNK(stat_buf.st_mode)) {
      throw ScanPortsException("Not a symlink");
  }
  
  // Read the symlink target
  ssize_t len = readlink(symlink_path.c_str(), buffer, sizeof(buffer) - 1);
  if (len < 0) {
    throw ScanPortsException("Failed to read symlink: " +
                              std::string(strerror(errno)));
  } else if (len >= static_cast<ssize_t>(sizeof(buffer))) {
    // Path too long - skip this entry
    throw ScanPortsException("Symlink path too long: " +
      std::string(strerror(errno)));
  }

  buffer[len] = '\0';
  target.assign(buffer, len);
}

}  // namespace libserial
