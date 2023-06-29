#pragma once
#include <j16/device.h>
#include <cstdint>

class Button {
	uint16_t signal;
	//bool analog;
public:
	Button(uint16_t);
	~Button();
};

class Gamepad : public Device {
	std::vector<Button> buttons;
public:
	Gamepad();
	~Gamepad();
	void send(const std::vector<uint16_t>&) override;
	std::vector<uint16_t> recv() override;
};