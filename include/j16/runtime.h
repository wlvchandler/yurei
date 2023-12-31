#pragma once

#include <cstdint>
#include <j16/gamepad.h>
#include <j16/isa.h>
#include <j16/memory.h>
#include <j16/sysinfo.h>

class Runtime {
    std::string appfile_name;
    std::ifstream appfile;
    Memory* main_memory;
    Endian sysendian;
    Endian srcendian;
  public:
    Runtime();
    ~Runtime();

    void start();
    void load_application();
    void LoadToROM();
};