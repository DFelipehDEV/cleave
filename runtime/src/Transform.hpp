#pragma once
#include "Vec2.hpp"

class Transform {
public:
    Transform(const Vec2f position = {0.0f, 0.0f}, const Vec2f scale = {1.0f, 1.0f}, float rotation = 0.0f, Transform* parent = nullptr) : m_position(position), m_scale(scale), m_rotation(rotation), m_parent(parent) {}
    ~Transform() = default;

    Vec2f GetPosition() const;
    void SetPosition(const Vec2f position);

    Vec2f GetScale() const;
    void SetScale(const Vec2f scale);

    float GetRotation() const;
    void SetRotation(float rotation);

    Transform* GetParent() const;
    void SetParent(Transform* transform);

    Vec2f GetWorldPosition() const;
private:
    Vec2f m_position;
    Vec2f m_scale;
    float m_rotation;

    Transform* m_parent;
};