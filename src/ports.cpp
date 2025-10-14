// @ Copyright 2020-2025 Nestor Neto

/**
 *
 *
 * \file serial.cpp
 */
#include <cstdio>
#include <string>
#include "libserial/ports.hpp"

namespace libserial {

Ports::Ports() {
}

Ports::~Ports() {
}

void Ports::list_ports() const {
  // Note: ports listing implementation is platform-specific and was
  // left as a placeholder. Remove unused variables reported by static
  // analysis. Implement actual device enumeration when needed.
  (void)device_list_;  // suppress unused member warning until implemented

  // std::cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

  // if (feof(fp_id)) {
  //   fscanf(fp_id, "%s", ls_output);
  //   std::cout << "1111111111111111111111111   " << std::endl;
  // }


  // std::cout << std::endl;

  // // Get device name
  // if (strcmp (ls_output, "total") == 0) {
  //   std::cout << "222222222222222222222" << std::endl;
  //   //fscanf(fp_id, "%s", ls_output);
  //   //fscanf(fp_id, "%s", ls_output);
  //   serial::device.id = 1;
  //   do {
  //     for (int i = 0; i < 8; i++) {
  //       //fscanf(fp_id, "%s", ls_output);
  //     }
  //     serial::device.name = ls_output;

  //     // Get port name
  //     //fscanf(fp_id, "%s", ls_output);
  //     //fscanf(fp_id, "%s", ls_output);
  //     serial::device.port = ls_output;
  //     serial::device.port.replace(0, 5, dev);

  //     // Get device path
  //     //fscanf(fp_path, "%s", ls_output);
  //     serial::device.path = ls_output;

  //     device_list_.push_back(serial::device);

  //     //fscanf(fp_id, "%s", ls_output);
  //     serial::device.id++;

  //   } while (strcmp (serial::device.path.c_str(), ls_output));
  //   // fclose(fp_id);
  //   //fclose(fp_path);
  // }
  // else {
  //   // std::cout << "33333333333333333333333333" << std::endl;
  //   // serial::device.path = "NaN";
  //   // serial::device.name = "NaN";
  //   // serial::device.port = "NaN";
  //   // fclose(fp_id);
  //   //fclose(fp_path);
  // }

  // std::cout << "Number of devices: " << device_list_.size() << std::endl;

  // for (auto i : device_list_ ) {
  //   std::cout << "Device ID: " << i.id<< std::endl;
  //   std::cout << "Device name: " << i.name<< std::endl;
  //   std::cout << "Device port: " << i.port<< std::endl;
  //   std::cout << "Device path: " << i.path<< std::endl;
  // }
}
}  // namespace libserial
