# Require a minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HardwareReader VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Define your source files
set(SOURCE_FILES 
    src/main.cpp 
    src/Hardware.cpp
)

# Create the executable
add_executable(hardware-reader ${SOURCE_FILES})

# Define where your header files are
target_include_directories(hardware-reader PRIVATE include)

# format
file(GLOB_RECURSE ALL_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_SOURCE_DIR}/src/*.hpp" 
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)
add_custom_target(format
    COMMAND clang-format -i ${ALL_FILES}
    COMMENT "Formatting all discovered source and header files..."
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

