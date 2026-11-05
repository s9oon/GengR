#pragma once

namespace GengR {
    struct Vec2 {
        float x = 0;
        float y = 0;
    };

    constexpr Vec2 operator+(Vec2 a, Vec2 b) { return { a.x + b.x, a.y + b.y }; }
    constexpr Vec2 operator-(Vec2 a, Vec2 b) { return { a.x - b.x, a.y - b.y }; }
    constexpr Vec2 operator*(Vec2 v, float s) { return { v.x * s, v.y * s }; }
    constexpr Vec2& operator+=(Vec2& a, Vec2 b) { a.x += b.x; a.y += b.y; return a; }
}