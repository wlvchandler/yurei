#pragma once

#include <j16/j16.h>
#include <j16/memory.h>

#include <cstdint>
#include <vector>
#include <iostream>

class Memory;

class Opcode {
    void NOP();
    void HALT();
    void LOAD();
    void STORE();
    void MOV();
    void ADD();
    void SUB();
    void MUL();
    void SMUL();
    void DIV();
    void SDIV();
    void AND();
    void OR();
    void XOR();
    void NOT();
    void SHR();
    void SHL();
    void JUMP();
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
        &Opcode::ADD, &Opcode::SUB, &Opcode::MUL, &Opcode::SMUL, &Opcode::DIV, &Opcode::SDIV,
        &Opcode::AND, &Opcode::OR, &Opcode::XOR, &Opcode::NOT, &Opcode::SHR, &Opcode::SHL,
        &Opcode::JUMP, &Opcode::JZ, &Opcode::JNZ, &Opcode::JC, &Opcode::JNC,
        &Opcode::CALL, &Opcode::RET, &Opcode::PUSH, &Opcode::POP
    };

public:
    Opcode();
    ~Opcode();

    void load(unsigned short);

private:
    std::vector<OpcodeFunction> instructions;
};
