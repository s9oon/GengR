#pragma once

#include <cstdint>

namespace GengR::Engine {
    struct Color {
        std::uint8_t r = 255;
        std::uint8_t g = 255;
        std::uint8_t b = 255;
        std::uint8_t a = 255; // 255 = solid, 0 = invisible
    };

    namespace Colors {
        inline constexpr Color White { 255, 255, 255, 255 };
        inline constexpr Color Black {   0,   0,   0, 255 };
        inline constexpr Color Red   { 230,  41,  55, 255 };
        inline constexpr Color Green {   0, 228,  48, 255 };
        inline constexpr Color Blue  {   0, 121, 241, 255 };
    }
}