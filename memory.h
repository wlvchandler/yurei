#pragma once

#include "utility/log.h"
#include "isa.h"
#include  <iostream>
#include  <iomanip>
#include  <string>

namespace REGISTERS {
    enum class REGISTER : unsigned char {
        A = 0, B, C, D, SP, PC, FLAGS, NUM_REGISTERS
    };

    #define Accessible(X) constexpr unsigned char X = static_cast<int>(REGISTER::X)
    Accessible(A);
    Accessible(B);
    Accessible(C);
    Accessible(D);
    Accessible(SP);
    Accessible(PC);
    Accessible(FLAGS);
    Accessible(NUM_REGISTERS);
    #undef Accessible

    using REG = REGISTER;
}

namespace SEGMENTS {
    enum class SEGMENT : unsigned char {
        DATA = 0, RAM, VRAM, SWAP, ROM, NUM_SEGMENTS
    };

#define Accessible(X) constexpr unsigned char X = static_cast<int>(SEGMENT::X)
    Accessible(DATA);
    Accessible(RAM); 
    Accessible(VRAM);
    Accessible(SWAP);
    Accessible(ROM);
    Accessible(NUM_SEGMENTS);
#undef Accessible
}

class Segment {
public:
    Segment(uint16_t, uint16_t);
    ~Segment();

    uint16_t    read(uint16_t);
    void        write(uint16_t, uint16_t);

private:
    uint16_t* base;
    uint16_t start;
    uint16_t size; 
};

/// memory segments
/// 0x0000 - 0x1FFF: System data - interrupt vectors, BIOS, stack. Stack starts at top (0x1FFF) and grows down.
/// 0x2000 - 0x5FFF: General RAM - variables/dynamically allocated data
/// 0x6000 - 0x9FFF: Sprite attribute table (OAM) - sprite data eg position/sz/palette/etc
/// 0xA000 - 0xDFFF: ROM/RAM switchable banks - switch between different memory.
/// 0xE000 - 0xFFFF: fixed ROM bank for program code
///

class Memory {
    friend class Opcode;

public:
    static Memory* getInstance();
    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;
  
    uint16_t* getMemory();

    uint16_t get_pc() const;
    uint16_t execute_memory();
    void check_for_interrupts();

private:
    Memory();
    ~Memory();


    void initializeSegments();

    void dump();
    void increment_pc();

    // Access value in register
    template <typename T>
    unsigned short& operator[](T& r) {
        return registers[static_cast<std::underlying_type_t<REGISTERS::REG>>(r)];
    }

    template <typename T>
    const unsigned short& operator[](T& r) const {
        return registers[static_cast<std::underlying_type_t<REGISTERS::REG>>(r)];
    }

private:
    const uint16_t ADDRESS_SPACE_SIZE = 65535;
    uint16_t* address_space;
    uint16_t* registers;

    std::vector<Segment> segments;

    static Memory* instance_;
};

