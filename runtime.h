#pragma once

#include "utility/sysinfo.h"
#include "isa.h"
#include "memory.h"
#include "device/gamepad.h"
#include <cstdint>

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