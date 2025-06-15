#pragma once
#include <cmath>

#include "Vec2.hpp"

class Matrix4 {
public:
    float m[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

    static Matrix4 Identity();

    Matrix4 operator*(const Matrix4& other) const;

    void Translate(Vec2f translation);
    void Scale(Vec2f scale);
    void Rotate(float radians);

    Vec2f GetPosition() const;
    void SetPosition(Vec2f position);

    Vec2f GetScale() const;
    void SetScale(Vec2f scale);

    float GetRotation() const;
    float GetRotationDegrees() const;
    void SetRotation(float radians);
    void SetRotationDegrees(float degrees);
};