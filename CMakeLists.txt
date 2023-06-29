cmake_minimum_required(VERSION 3.14)
project(jaskASM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DTEST_FILE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")

include(FetchContent)
FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.28.0)
FetchContent_MakeAvailable(SDL2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB J16ASM_FILES "src/j16asm/*.cpp" "include/j16/{isa,assembler,log,sysinfo}.h" "src/{log,isa}.cpp")
file(GLOB J16PS_FILES "src/j16/*.cpp" "include/j16/*.h" "src/*.cpp")
file(GLOB YUREI_FILES "src/*.cpp" "include/j16/*.h")

add_executable(yurei
    include/j16/yurei.h
    include/j16/memory.h
    include/j16/isa.h
    include/j16/runtime.h
    include/j16/log.h
    include/j16/sysinfo.h
    include/j16/gamepad.h
    src/j16/runtime.cpp
    src/isa.cpp
    src/log.cpp
    src/memory.cpp
    src/yurei.cpp
)

add_executable(j16asm ${J16ASM_FILES})
add_executable(j16ps ${J16PS_FILES})
#add_executable(yurei ${YUREI_FILES})

target_link_libraries(j16ps PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(yurei PRIVATE SDL2::SDL2 SDL2::SDL2main)
