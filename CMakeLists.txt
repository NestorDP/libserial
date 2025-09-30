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

# Documentation configuration
option(BUILD_DOCUMENTATION "Build documentation" OFF)

if(BUILD_DOCUMENTATION)
    # Find Doxygen
    find_package(Doxygen REQUIRED)
    
    # Find Sphinx
    find_program(SPHINX_EXECUTABLE
        NAMES sphinx-build
        DOC "Path to sphinx-build executable"
    )
    
    if(NOT SPHINX_EXECUTABLE)
        message(FATAL_ERROR "sphinx-build not found. Install sphinx or set BUILD_DOCUMENTATION=OFF")
    endif()
    
    # Configure Doxygen
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    
    # Create doxygen target
    add_custom_target(doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    
    # Configure Sphinx
    set(SPHINX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs)
    set(SPHINX_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(SPHINX_HTML_DIR ${SPHINX_BUILD_DIR}/html)
    
    # Create sphinx target
    add_custom_target(sphinx
        COMMAND ${SPHINX_EXECUTABLE} -b html ${SPHINX_SOURCE_DIR} ${SPHINX_HTML_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS doxygen
        COMMENT "Generating documentation with Sphinx"
        VERBATIM
    )
    
    # Create combined documentation target
    add_custom_target(docs
        DEPENDS sphinx
        COMMENT "Building complete documentation"
    )
    
    # Install documentation
    install(DIRECTORY ${SPHINX_HTML_DIR}/
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/html
        OPTIONAL
    )
endif()

# Testing configuration
option(BUILD_TESTING "Build tests" OFF)

if(BUILD_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)

    add_executable(serial_tests
        test/test_serial_simple.cpp
        test/test_serial_pty.cpp
    )
    
    target_link_libraries(serial_tests PRIVATE
        ${PROJECT_NAME}
        GTest::gtest
        GTest::gtest_main
        pthread
    )

    add_test(NAME SerialTests COMMAND serial_tests)
endif()