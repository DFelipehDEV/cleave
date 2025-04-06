#include "Matrix4.hpp"

Matrix4 Matrix4::Identity() {
    return Matrix4();
}

Matrix4 Matrix4::CreateTransform(Vec2f position, float rotation, Vec2f scale) {
    Matrix4 mat;
    mat.Translate(position);
    mat.Rotate(rotation);
    mat.Scale(scale);
    return mat;
}

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

void Matrix4::Scale(Vec2f scale) {
    Matrix4 scaling;
    scaling.m[0][0] = scale.x;
    scaling.m[1][1] = scale.y;
    *this = *this * scaling;
}

Vec2f Matrix4::GetPosition() const {
    return Vec2f(m[3][0], m[3][1]);
}

float Matrix4::GetRotation() const {
    return std::atan2(m[1][0], m[0][0]);
}

Vec2f Matrix4::GetScale() const {
    return Vec2f(
        std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]),
        std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1])
    );
}