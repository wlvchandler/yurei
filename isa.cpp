#include "isa.h"

Opcode::Opcode() {
    main_memory = Memory::getInstance();
    for (const auto& func : opcodeFunctions) {
        instructions.push_back(func);
    }
}

Opcode::~Opcode() {}

void Opcode::load(uint16_t instruction) {
    std::cout << "Loading " << instruction << std::endl;
    if (instruction <= static_cast<uint16_t>(Mnemonic::N_OPCODES)) {
        (this->*opcodeFunctions[instruction])();
    } // else std::cout << "No opcode for instruction 0x" << std::hex << instruction << std::dec << std::endl;
}

void Opcode::NOP() { std::cout << "NOP\n"; }
void Opcode::HALT() { std::cout << "HALT\n"; }
void Opcode::LOAD() {
    std::cout << "LOAD\n";
}
void Opcode::STORE() {}
void Opcode::MOV() {}
void Opcode::ADD() {}
void Opcode::SUB() {}
void Opcode::MUL() {}
void Opcode::SMUL() {}
void Opcode::DIV() {}
void Opcode::SDIV() {}
void Opcode::AND() {}
void Opcode::OR() {}
void Opcode::XOR() {}
void Opcode::NOT() {}
void Opcode::SHR() {}
void Opcode::SHL() {}
void Opcode::JUMP() {}
void Opcode::JZ() {}
void Opcode::JNZ() {}
void Opcode::JC() {}
void Opcode::JNC() {}
void Opcode::CALL() {}
void Opcode::RET() {}
void Opcode::PUSH() {}
void Opcode::POP() {}