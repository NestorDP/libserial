// @ Copyright 2020

/**
 * 
 *
 * \file serial.cpp
 */

#include "libserial/ports.hpp"

serial::Ports::Ports() {
  port_list_ = new std::vector<std::string>();
}

serial::Ports::~Ports() { 

}

void serial::Ports::list_ports() {
  char device_name[100];

  // List available serial ports 
  FILE *fp;
  const char *command = "ls -l /dev/serial/by-id";
  fp = popen(command,"r"); 
  fscanf(fp, "%s", device_name);
  

  // Get device name
  if (strcmp (device_name, "total\n")) {
    for (int i = 0; i <10; i++) {
      fscanf(fp, "%s", device_name);
    }
  }
  printf("%s\n", device_name);

  // Get port name
  fscanf(fp, "%s", device_name);
  fscanf(fp, "%s", device_name);
  printf("%s\n", device_name);


 

  fclose(fp);
}
