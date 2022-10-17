// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/ports.hpp"

serial::Ports::Ports() {
  port_list_ = new std::vector<std::string>();
  cmd_= "ls -l /dev/serial/by-id";
}

serial::Ports::~Ports() { 

}

void serial::Ports::list_ports() {
  FILE *fp_;
  char device_name_[100];


  fp_ = popen(cmd_,"r"); 
  fscanf(fp_, "%s", device_name_);

  // port_list_->push_back(device_name_);
  //std::cout << port_list_->at(0) << std::endl;

  // Get device name
  if (strcmp (device_name_, "total\n")) {
    for (int i = 0; i <10; i++) {
      fscanf(fp_, "%s", device_name_);
    }
  }

  port_list_->push_back(device_name_);
  //std::cout << port_list_->at(1) << std::endl;

  // // Get port name
  fscanf(fp_, "%s", device_name_);
  fscanf(fp_, "%s", device_name_);

  port_list_->push_back(device_name_);

  for(auto x: *port_list_) {
    std::cout << x << std::endl;
  }

  fclose(fp_);
}
