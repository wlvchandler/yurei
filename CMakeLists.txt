cmake_minimum_required(VERSION 3.8)

project(jaskASM LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source files
set(SOURCE_FILES
    "jaskASM.cpp"
    "jaskASM.h"
    "memory.h"
    "memory.cpp"
    "isa.h"
    "isa.cpp"
    "runtime.h"
    "runtime.cpp"
    "assembler.h"
    "assembler.cpp"
    "device/device.h"
    "device/device.cpp"
    "device/gamepad.h"
    "device/gamepad.cpp"
    "utility/log.cpp"
    "utility/log.h"
    "utility/sysinfo.h"
)


add_executable(jaskASM ${SOURCE_FILES})
