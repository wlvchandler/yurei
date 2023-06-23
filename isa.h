#pragma once

#include "memory.h"
#include <cstdint>
#include <vector>
#include <iostream>

class Memory;

enum class Instruction : uint16_t {
    NOP  = 0x0000, HALT  = 0x0001, 
    LOAD = 0x000A, STORE = 0x000B, MOV = 0x000C, PUSH = 0x000D, POP = 0x000E, 
    ADD  = 0x0010, SUB   = 0x0011, MUL = 0x0012, DIV  = 0x0013, 
    AND  = 0x0020, OR    = 0x0021, XOR = 0x0022, NOT  = 0x0023, 
    JUMP = 0x0030, JZ    = 0x0031, JNZ = 0x0032, JC   = 0x0033, JNC  = 0x0034,
    CALL = 0x0040, RET   = 0x0041, 
    A    = 0x00F0, B     = 0x00F1, C    = 0x00F2, D   = 0x00F3
};


class Opcode {
    void NOP();
    void HALT();
    void LOAD();
    void STORE();
    void MOV();
    void ADD();
    void SUB();
    void MUL();
    void DIV();
    void AND();
    void OR();
    void XOR();
    void NOT();
    void JMP();
    void JZ();
    void JNZ();
    void JC();
    void JNC();
    void CALL();
    void RET();
    void PUSH();
    void POP();

    using OpcodeFunction = void (Opcode::*)();
    static constexpr OpcodeFunction opcodeFunctions[] = {
        &Opcode::NOP, &Opcode::HALT, 
        &Opcode::LOAD, &Opcode::STORE, &Opcode::MOV,
        &Opcode::ADD, &Opcode::SUB, &Opcode::MUL, &Opcode::DIV,
        &Opcode::AND, &Opcode::OR, &Opcode::XOR, &Opcode::NOT,
        &Opcode::JMP, &Opcode::JZ, &Opcode::JNZ, &Opcode::JC, &Opcode::JNC,
        &Opcode::CALL, &Opcode::RET, &Opcode::PUSH, &Opcode::POP
    };

public:
    Opcode();
    ~Opcode();

    void load(unsigned short);

private:
    std::vector<OpcodeFunction> instructions;
    Memory* main_memory;
};
