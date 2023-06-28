#pragma once
#include <cstdint>
#include <vector>

class Device {
public:
	virtual ~Device() = default;
	virtual void send(const std::vector<uint16_t>&) = 0;
	virtual std::vector<uint16_t> recv() = 0;
};
