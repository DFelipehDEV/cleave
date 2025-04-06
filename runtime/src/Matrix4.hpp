#pragma once
#include "Vec2.hpp"
#include <cmath>

class Matrix4 {
public:
    // Initialize as identity matrix
    float m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // Factory methods
    static Matrix4 Identity();
    static Matrix4 CreateTransform(Vec2f position, float rotation = 0.0f, Vec2f scale = {1.0f, 1.0f});

    // Operations
    Matrix4 operator*(const Matrix4& other) const;
    
    // Transformations
    void Translate(Vec2f translation);
    void Rotate(float radians);
    void Scale(Vec2f scale);

    // Getters
    Vec2f GetPosition() const;
    float GetRotation() const;
    Vec2f GetScale() const;
};