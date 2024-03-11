#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>

#ifdef TEST_FILE_DIR
#define TESTS_DIR TEST_FILE_DIR
#else
#define TESTS_DIR "test"
#endif

// mnemonics
enum class Mnemonic : uint16_t {
    N_OPCODES = 26,
    NOP = 0x0000,
    HALT = 0x0001,
    LOAD = 0x000A,
    STORE = 0x000B,
    MOV = 0x000C,
    PUSH = 0x000D,
    POP = 0x000E,
    ADD = 0x0010,
    SUB = 0x0011,
    MUL = 0x0012,
    SMUL = 0x0013,
    DIV = 0x0014,
    SDIV = 0x0015,
    AND = 0x0020,
    OR = 0x0021,
    XOR = 0x0022,
    NOT = 0x0023,
    SHR = 0x0024,
    SHL = 0x0025,
    JUMP = 0x0030,    
    JZ = 0x0031,
    JNZ = 0x0032,
    JC = 0x0033,
    JNC = 0x0034,
    CALL = 0x0040,
    RET = 0x0041,
    A = 0x00F0,
    B = 0x00F1,
    C = 0x00F2,
    D = 0x00F3,
    I = 0x00F4,
    J = 0x00F5,
    SP = 0x00F6,
    PC = 0x00F7,
    FLAGS = 0x00F8
};

extern uint16_t ROMtest[];