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
    return []() {
        constexpr uint16_t value = 0x0001;
        return reinterpret_cast<const uint8_t&>(value) == 0x01;
    }();
}

inline uint16_t correctEndianness(uint16_t value, Endian sourceEndianness) {
    return ((sourceEndianness == Endian::Big) == isSystemLittleEndian()) ? swap_bytes(value) : value;
}