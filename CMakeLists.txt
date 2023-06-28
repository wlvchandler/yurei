cmake_minimum_required(VERSION 3.8)
project(jaskASM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions( -DTEST_FILE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
set(SOURCE_FILES
    "src/jaskASM.cpp"
    "include/j16/jaskASM.h"
    "include/j16/memory.h"
    "src/memory.cpp"
    "include/j16/isa.h"
    "src/isa.cpp"
    "include/j16/runtime.h"
    "src/j16/runtime.cpp"
    "include/j16/assembler.h"
    "src/j16asm/assembler.cpp"
    "include/j16/device.h"
    "src/device/device.cpp"
    "include/j16/gamepad.h"
    "src/device/gamepad.cpp"
    "src/log.cpp"
    "include/j16/log.h"
    "include/j16/sysinfo.h"
)

set(UTIL_FILES
    "src/log.cpp"
    "include/j16/log.h"
    "include/j16/sysinfo.h"
)

set(ASM_FILES
    "include/j16/isa.h"
    "src/isa.cpp"
    "src/j16asm/assembler.cpp"
    "include/j16/assembler.h"
)

set(RUNTIME_FILES
    "include/j16/runtime.h"
    "src/j16/runtime.cpp"
)


add_executable(jaskASM ${SOURCE_FILES})

 #enable later
#add_executable(j16asm ${UTIL_FILES} ${ASM_FILES})
#add_executable(j16ps  ${UTIL_FILES} ${RUNTIME_FILES})