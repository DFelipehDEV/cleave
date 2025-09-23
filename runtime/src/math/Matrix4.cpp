#define _USE_MATH_DEFINES
#include "Matrix4.hpp"

#include <cmath>

namespace Cleave {
Matrix4 Matrix4::Identity() { return Matrix4(); }

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = 0.0f;
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (zFar - zNear);
    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -(zFar + zNear) / (zFar - zNear);
    result.m[3][3] = 1.0f;
    return result;
}
}  // namespace Cleave