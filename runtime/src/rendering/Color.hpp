#pragma once
#include <cstdint>

namespace Cleave {
struct Color {
    uint8_t r, g, b, a;
    Color() : r(0), g(0), b(0), a(255) {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    static Color White()   { return Color(255, 255, 255, 255); }
    static Color Black()   { return Color(0, 0, 0, 255); }
    static Color Red()     { return Color(255, 0, 0, 255); }
    static Color Green()   { return Color(0, 255, 0, 255); }
    static Color Blue()    { return Color(0, 0, 255, 255); }
    static Color Yellow()  { return Color(255, 255, 0, 255); }
    static Color Cyan()    { return Color(0, 255, 255, 255); }
    static Color Magenta() { return Color(255, 0, 255, 255); }
    static Color Gray()    { return Color(128, 128, 128, 255); }
};
}  // namespace Cleave