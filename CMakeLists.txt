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

# classes
set(hdrs "include/j16")
set(common_hdr "${hdrs}/j16.h" "${hdrs}/sysinfo.h")
set(classes assembler isa log memory runtime device gamepad)
foreach(c ${classes})
	set(src_${c} "${hdrs}/${c}.h" "src/${c}.cpp")
endforeach()
set(src_common    ${common_hdr} ${src_log})
set(src_hardware  ${src_memory} ${src_device} ${src_gamepad} )

#### build targets
# assembler
add_executable(oni16 "src/j16asm/j16asm.cpp"
	${src_common} ${src_assembler} ${src_isa} 
)

# hardware runtime
add_executable(yurei "src/yurei.cpp"  
	${src_common} ${src_hardware} ${src_runtime}  ${src_isa}  
)

target_link_libraries(yurei PRIVATE SDL2::SDL2 SDL2::SDL2main)