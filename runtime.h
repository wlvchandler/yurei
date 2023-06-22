#pragma once

#include "isa.h"
#include "memory.h"
#include "device/gamepad.h"

class Runtime {
	Memory* main_memory;

public:
	Runtime();
	~Runtime();

	void start();
	void load_application();
};