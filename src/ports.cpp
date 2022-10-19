// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/ports.hpp"

serial::Ports::Ports() {
  
}

serial::Ports::~Ports() { 

}

void serial::Ports::list_ports() {
  FILE *fp_id;
  FILE *fp_path;
  char ls_output[100];
  std::string dev = "/dev";

  fp_id = popen("ls -l /dev/serial/by-id","r"); 
  fp_path = popen("ls /dev/serial/by-path","r"); 
  fscanf(fp_id, "%s", ls_output);

  std::cout << std::endl;

  // Get device name
  if (strcmp (ls_output, "total") == 0) {
    fscanf(fp_id, "%s", ls_output);
    fscanf(fp_id, "%s", ls_output);
    serial::device.id = 1;
    do {
      for (int i = 0; i < 8; i++) {
        fscanf(fp_id, "%s", ls_output);
      }
      serial::device.name = ls_output;

      // Get port name
      fscanf(fp_id, "%s", ls_output);
      fscanf(fp_id, "%s", ls_output);
      serial::device.port = ls_output;
      serial::device.port.replace(0, 5, dev);

      // Get device path
      fscanf(fp_path, "%s", ls_output);
      serial::device.path = ls_output;

      device_list_.push_back(serial::device);

      fscanf(fp_id, "%s", ls_output);
      serial::device.id++;

    } while (strcmp (serial::device.path.c_str(), ls_output));
 
    
    fclose(fp_id);
    fclose(fp_path);
  }
  else {
    serial::device.path = "NaN";
    serial::device.name = "NaN";
    serial::device.port = "NaN";
  }  

  std::cout << "Number of devices" << device_list_.size() << std::endl;

  for (auto i : device_list_ ) {
    std::cout << "Device ID: " << i.id<< std::endl;
    std::cout << "Device name: " << i.name<< std::endl;
    std::cout << "Device port: " << i.port<< std::endl;
    std::cout << "Device path: " << i.path<< std::endl;
  }

}
