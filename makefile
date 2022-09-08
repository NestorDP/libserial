PROJECT = libserial
CC = g++
# ARCH = arm
CFLAGS = -g -Wall -Werror -fpic -std=c++14
LDFLAGS = -g -Wall -shared

LIBDIR = lib
BUILDDIR = build
INCLUDEDIR = include
SOURCEDIR = src

SOURCES = $(shell find $(SOURCEDIR) -type f -name *.cpp)
OBJECTS	= $(patsubst $(SOURCEDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
INCLUDES = -I $(INCLUDEDIR)

TARGET = $(LIBDIR)/$(PROJECT).so

$(TARGET): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a $(BUILDDIR)/*.o *.~

.PHONY: install
install:
	sudo cp $(TARGET) /usr/lib 
	sudo cp -R $(INCLUDEDIR)/$(PROJECT)/ /usr/include/
