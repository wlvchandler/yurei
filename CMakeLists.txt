cmake_minimum_required(VERSION 3.8)
project(jaskASM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions( -DTEST_FILE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#if (WIN32)
#    set(SDL2_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2.lib") 
#elseif (UNIX)
#    set(SDL2_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2.a") 
# endif()


# Add source files
set(SOURCE_FILES
    "src/yurei.cpp"
    "include/j16/yurei.h"
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
    "src/j16asm/j16asm.cpp"
    "include/j16/isa.h"
    "src/isa.cpp"
    "src/j16asm/assembler.cpp"
    "include/j16/assembler.h"
)

set(RUNTIME_FILES
    "src/j16/j16ps.cpp"
    "include/j16/runtime.h"
    "src/j16/runtime.cpp"
    "include/j16/memory.h"
    "src/memory.cpp"
    "include/j16/isa.h"
    "src/isa.cpp"
)

add_executable(jaskASM ${SOURCE_FILES})
add_executable(j16asm ${UTIL_FILES} ${ASM_FILES})
add_executable(j16ps  ${UTIL_FILES} ${RUNTIME_FILES})