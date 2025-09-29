#pragma once
#include <cmath>

#include "math/Vec2.hpp"

namespace Cleave {
class Matrix4 {
public:
    float m[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

    static Matrix4 Identity();
    static Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    Matrix4 operator*(const Matrix4& other) const;
};
}  // namespace Cleave