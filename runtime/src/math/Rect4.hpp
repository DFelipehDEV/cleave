#pragma once

namespace Cleave {
struct Rect4f {
    float x, y, width, height;

    Rect4f(float _x, float _y, float _width, float _height)
        : x(_x), y(_y), width(_width), height(_height) {}
};
}  // namespace Cleave