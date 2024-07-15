#pragma once
#include <cstdint>

enum class Endian {
    Big,
    Little
};

inline uint16_t swap_bytes(uint16_t x) {
    return (x << 8) | (x >> 8);
}

constexpr bool isSystemLittleEndian() {
    union {
        uint16_t i;
        char c[2];
    } u = { 0x0100 };
    return u.c[0] != 0;
}

inline uint16_t correctEndianness(uint16_t value, Endian sourceEndianness) {
    return ((sourceEndianness == Endian::Big) == isSystemLittleEndian()) ? swap_bytes(value) : value;
}