#pragma once
#include <cstdint>

enum class Endian {
    Big,
    Little
};

inline uint16_t swap_bytes(uint16_t x) {
    return (x << 8) | (x >> 8);
}

static inline bool isSystemLittleEndian() {
    uint16_t x = 1;
    char* byte = reinterpret_cast<char*>(&x);
    return *byte == 1;
}

inline uint16_t correctEndianness(uint16_t value, Endian sourceEndianness) {
    return ((sourceEndianness == Endian::Big) == isSystemLittleEndian()) ? swap_bytes(value) : value;
}