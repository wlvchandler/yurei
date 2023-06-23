#pragma once

#include "memory.h"
#include <cstdint>
#include <vector>
#include <iostream>

class Memory;

enum class Instruction : unsigned char {
    NOP, HALT, LOAD, STORE, MOV, ADD, SUB, MUL, DIV, AND, OR, XOR, NOT,
    JMP, JZ, JNZ, JC, JNC, CALL, RET, PUSH, POP, N_OPCODES
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
