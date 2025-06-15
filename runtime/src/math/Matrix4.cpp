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

void Matrix4::Translate(Vec2f translation) {
    m[3][0] += translation.x;
    m[3][1] += translation.y;
}

void Matrix4::Scale(Vec2f scale) {
    Matrix4 scaling;
    scaling.m[0][0] = scale.x;
    scaling.m[1][1] = scale.y;
    *this = *this * scaling;
}

void Matrix4::Rotate(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);
    Matrix4 rotation;
    rotation.m[0][0] = c;
    rotation.m[0][1] = -s;
    rotation.m[1][0] = s;
    rotation.m[1][1] = c;
    *this = *this * rotation;
}

Vec2f Matrix4::GetPosition() const { return Vec2f(m[3][0], m[3][1]); }
void Matrix4::SetPosition(Vec2f position) {
    m[3][0] = position.x;
    m[3][1] = position.y;
}

Vec2f Matrix4::GetScale() const {
    return Vec2f(std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]),
                 std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1]));
}

void Matrix4::SetScale(Vec2f scale) {
    float currentScaleX = std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
    float currentScaleY = std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1]);
    if (currentScaleX != 0.0f) {
        m[0][0] /= currentScaleX;
        m[1][0] /= currentScaleX;
    }
    if (currentScaleY != 0.0f) {
        m[0][1] /= currentScaleY;
        m[1][1] /= currentScaleY;
    }

    m[0][0] *= scale.x;
    m[1][0] *= scale.x;
    m[0][1] *= scale.y;
    m[1][1] *= scale.y;
}

float Matrix4::GetRotation() const { return std::atan2(m[1][0], m[0][0]); }

float Matrix4::GetRotationDegrees() const {
    return GetRotation() * (180.0f / M_PI);
}

void Matrix4::SetRotation(float radians) {
    Vec2f scale = GetScale();
    float c = std::cos(radians);
    float s = std::sin(radians);

    m[0][0] = c * scale.x;
    m[0][1] = -s * scale.y;
    m[1][0] = s * scale.x;
    m[1][1] = c * scale.y;
}

void Matrix4::SetRotationDegrees(float degrees) {
    SetRotation(degrees * (M_PI / 180.0f));
}
}  // namespace Cleave