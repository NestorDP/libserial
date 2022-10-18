// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/ports.hpp"

serial::Ports::Ports() {
  list_ = new std::vector<std::string>();
  cmd_= "ls -l /dev/serial/by-id";
}

serial::Ports::~Ports() { 

}

void serial::Ports::list_ports() {
  FILE *fp_;
  char ls_output_[100];
  std::string dev = "/dev";


  fp_ = popen("ls -l /dev/serial/by-id","r"); 
  fscanf(fp_, "%s", ls_output_);

  // Get device name
  if (strcmp (ls_output_, "total\n")) {
    for (int i = 0; i <10; i++) {
      fscanf(fp_, "%s", ls_output_);
    }
  }

  serial::device.name = ls_output_;


  // Get port name
  fscanf(fp_, "%s", ls_output_);
  fscanf(fp_, "%s", ls_output_);

  serial::device.port = ls_output_;

  serial::device.port.replace(0, 5, dev);

  std::cout << serial::device.port << ' ' << serial::device.name << std::endl;
  
  fclose(fp_);
}
