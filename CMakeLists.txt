cmake_minimum_required(VERSION 3.10)

# Project configuration
project(libserial VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Enable position independent code for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include directories
include_directories(include)

# Find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME ${PROJECT_NAME}
)

# Installation configuration
include(GNUInstallDirs)

# Install the library
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.hpp"
)

# Optional: Create a pkg-config file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/libserial.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libserial.pc"
    @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libserial.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)