#pragma once
#include <cstdint>

#include <type_traits>

enum class Endian {
    Big,
    Little
};

inline uint16_t swap_bytes(uint16_t x) {
    return (x << 8) | (x >> 8);
}

namespace detail {
    template <typename T>
    struct endianness_impl {
        static constexpr T test_value = static_cast<T>(0x0102);
        static constexpr bool is_little = 
            (static_cast<std::uint8_t>(test_value) == 0x02);
    };
}

template <typename T = std::uint16_t>
constexpr bool isSystemLittleEndian() {
    static_assert(std::is_integral<T>::value && !std::is_same<T, bool>::value,
                  "T must be a non-bool integral type");
    static_assert(sizeof(T) > 1, "T must be at least 16 bits");
    return detail::endianness_impl<T>::is_little;
}

inline uint16_t correctEndianness(uint16_t value, Endian sourceEndianness) {
    return ((sourceEndianness == Endian::Big) == isSystemLittleEndian()) ? swap_bytes(value) : value;
}