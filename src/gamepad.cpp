#include <j16/gamepad.h>

Button::Button(uint16_t s) : signal(s) {}
Button::~Button() {}

Gamepad::Gamepad() {
    buttons.push_back(Button(0x154)); // A
    buttons.push_back(Button(0x155)); // B
    buttons.push_back(Button(0x156)); // UP
    buttons.push_back(Button(0x157)); // DOWN
    buttons.push_back(Button(0x158)); // LEFT
    buttons.push_back(Button(0x159)); // RIGHT
}

Gamepad::~Gamepad() {}

void Gamepad::send(const std::vector<uint16_t>& data) {}

std::vector<uint16_t> Gamepad::recv() {
    return std::vector<uint16_t>();
}