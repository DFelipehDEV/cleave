#pragma once
#include <cmath>
#include <string>

namespace Cleave {
template<typename T>
struct Vec2 {
    T x, y;

    Vec2(T _x, T _y) : x(_x), y(_y) {}

    static Vec2<T> Zero() { return {T(0), T(0)}; }

    Vec2<T> operator-() const { return Vec2<T>{-x, -y}; }

    // Arithmetic operators
    Vec2<T> operator+(const Vec2<T>& other) const {
        return Vec2<T>{x + other.x, y + other.y};
    }

    Vec2<T> operator-(const Vec2<T>& other) const {
        return Vec2<T>{x - other.x, y - other.y};
    }

    Vec2<T> operator*(const Vec2<T>& other) const {
        return Vec2<T>(x * other.x, y * other.y);
    }

    Vec2<T> operator/(const Vec2<T>& other) const {
        if (other.x != 0 && other.y != 0) {
            return Vec2<T>(x / other.x, y / other.y);
        }
        return Vec2<T>::Zero();
    }

    Vec2<T> operator*(T scalar) const {
        return Vec2<T>(x * scalar, y * scalar);
    }

    Vec2<T> operator/(T scalar) const {
        if (scalar != 0) {
            return Vec2<T>(x / scalar, y / scalar);
        }
        return Vec2<T>::Zero();
    }

    // Compound assignment operators
    Vec2<T>& operator+=(const Vec2<T>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2<T>& operator-=(const Vec2<T>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2<T>& operator*=(const Vec2<T>& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vec2<T>& operator/=(const Vec2<T>& other) {
        if (other.x != 0 && other.y != 0) {
            x /= other.x;
            y /= other.y;
            return *this;
        }
        return Vec2<T>::Zero();
    }

    Vec2<T>& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2<T>& operator/=(T scalar) {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
            return *this;
        }
        return Vec2<T>::Zero();
    }

    // Comparison operators
    bool operator==(const Vec2<T>& other) const {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Vec2<T>& other) const { return !(*this == other); }

    // Utility functions
    float Dot(const Vec2<T>& other) const { return x * other.x + y * other.y; }

    float MagnitudeSquared() const { return x * x + y * y; }

    float Magnitude() const { return std::sqrt(x * x + y * y); }

    Vec2<T> Perpendicular() const { return Vec2<T>(-y, x); }

    Vec2<T> Normalized() const {
        float mag = Magnitude();
        if (mag != 0) {
            return *this / mag;
        } else {
            return *this;
        }
    }

    Vec2<T> Rotate(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vec2<T>(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    std::string ToString() const {
        return std::to_string(x) + "," + std::to_string(y);
    }

    static Vec2<T> FromString(const std::string& str) {
        size_t comma = str.find(',');
        if (comma == std::string::npos) {
            return Vec2<T>::Zero();
        }
        try {
            float x = std::stof(str.substr(0, comma));
            float y = std::stof(str.substr(comma + 1));
            return Vec2<T>(x, y);
        } catch (const std::exception&) {
            return Vec2<T>::Zero();
        }
    }
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
}  // namespace Cleave