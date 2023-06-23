# CMakeList.txt : CMake project for jaskASM, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
project ("jaskASM")

# Add source to this project's executable.
add_executable (jaskASM "jaskASM.cpp" "jaskASM.h" "memory.h" "memory.cpp" "isa.h" "isa.cpp"  "runtime.h" "runtime.cpp" "assembler.h" "assembler.cpp" "device/device.h" "device/device.cpp" "device/gamepad.h" "device/gamepad.cpp" "utility/log.cpp" "utility/log.h" "utility/sysinfo.h")



# TODO: Add tests and install targets if needed.
