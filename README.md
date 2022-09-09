# libserial
C++ Application Programming Interface to termios.h C library 

The goal this API is to provide to developers an way to use the old termios.h C library through a modern C++ class. Concepts as Oriented-Object Programming, Smart Pointers and Templates are using here to give you more flexibilit and powerfull to yor aplication that need serial communication in a Linux enviroment. 


## Build and instalation
Clone the repository.
``` console
foo@bar:~$ git clone https://github.com/NestorDP/libserial.git
foo@bar:~$ cd libserial
```

Building the source code. If you don't have any problem you will see these mensagens
``` console
foo@bar:~$ make
g++ -g -Wall -Werror -fpic -std=c++14 -I include -c src/serial.cpp -o build/serial.o
g++ -g -Wall -shared build/serial.o -o lib/libserial.so
```

Instal the library. You will to need put your sudo password to finish this step.
``` console
foo@bar:~$ make install
sudo cp lib/libserial.so /usr/lib 
[sudo] password for foo: 
sudo cp -R include/libserial/ /usr/include/
```
