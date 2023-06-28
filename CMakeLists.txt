cmake_minimum_required(VERSION 3.8)

project(jaskASM LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
     "${CMAKE_SOURCE_DIR}/*.cpp"
     "${CMAKE_SOURCE_DIR}/*.h")

add_executable(jaskASM ${SOURCE_FILES})
