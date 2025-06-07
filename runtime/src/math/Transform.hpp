#pragma once
#include "math/Vec2.hpp"
#include "math/Matrix4.hpp"

class Transform : public Matrix4 {
public:
    Transform(const Vec2f position = {0.0f, 0.0f}, const Vec2f scale = {1.0f, 1.0f}, float rotation = 0.0f, Transform* parent = nullptr);
    ~Transform() = default;

    Transform* GetParent() const;
    void SetParent(Transform* transform);

    Vec2f GetWorldPosition() const;
    Vec2f GetWorldScale() const;
    float GetWorldRotation() const;
private:
    Transform* m_parent;
};