#pragma once
#include <cmath>
#include <string>

struct Vec2f {
    float x, y;

    Vec2f(float _x, float _y) : x(_x), y(_y) {}

    static Vec2f Zero() { return {0.0f, 0.0f}; }

    Vec2f operator-() const { return Vec2f{-x, -y}; }

    // Arithmetic operators
    Vec2f operator+(const Vec2f& other) const {
        return Vec2f{x + other.x, y + other.y};
    }

    Vec2f operator-(const Vec2f& other) const {
        return Vec2f{x - other.x, y - other.y};
    }

    Vec2f operator*(const Vec2f& other) const {
        return Vec2f(x * other.x, y * other.y);
    }

    Vec2f operator/(const Vec2f& other) const {
        if (other.x != 0 && other.y != 0) {
            return Vec2f(x / other.x, y / other.y);
        }
        return Vec2f::Zero();
    }

    Vec2f operator*(float scalar) const {
        return Vec2f(x * scalar, y * scalar);
    }

    Vec2f operator/(float scalar) const {
        if (scalar != 0) {
            return Vec2f(x / scalar, y / scalar);
        }
        return Vec2f::Zero();
    }

    // Compound assignment operators
    Vec2f& operator+=(const Vec2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2f& operator-=(const Vec2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2f& operator*=(const Vec2f& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vec2f& operator/=(const Vec2f& other) {
        if (other.x != 0 && other.y != 0) {
            x /= other.x;
            y /= other.y;
            return *this;
        }
        return Vec2f::Zero();
    }

    Vec2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2f& operator/=(float scalar) {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
            return *this;
        }
        return Vec2f::Zero();
    }

    // Comparison operators
    bool operator==(const Vec2f& other) const {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Vec2f& other) const { return !(*this == other); }

    // Utility functions

    float Dot(const Vec2f& other) const { return x * other.x + y * other.y; }

    float MagnitudeSquared() const { return x * x + y * y; }

    float Magnitude() const { return std::sqrt(x * x + y * y); }

    Vec2f Perpendicular() const { return Vec2f(-y, x); }

    Vec2f Normalized() const {
        float mag = Magnitude();
        if (mag != 0) {
            return *this / mag;
        } else {
            return *this;
        }
    }

    Vec2f Rotate(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vec2f(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    std::string ToString() const {
        return std::to_string(x) + "," + std::to_string(y);
    }

    static Vec2f FromString(const std::string& str) {
        size_t comma = str.find(',');
        if (comma == std::string::npos) {
            return Vec2f::Zero();
        }
        try {
            float x = std::stof(str.substr(0, comma));
            float y = std::stof(str.substr(comma + 1));
            return Vec2f(x, y);
        } catch (const std::exception&) {
            return Vec2f::Zero();
        }
    }
};