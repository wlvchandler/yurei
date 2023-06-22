#include "runtime.h"

Runtime::Runtime() {
	this->main_memory = Memory::getInstance();
	std::cout << "memory instance loaded\n";
}

Runtime::~Runtime() {}


void Runtime::start() {
	// Load application
	// execute
}

void Runtime::load_application() {}