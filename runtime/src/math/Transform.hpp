#pragma once
#include "math/Matrix4.hpp"
#include "math/Vec2.hpp"

namespace Cleave {
class Transform {
public:
    Transform(const Vec2f position = {0.0f, 0.0f},
              const Vec2f scale = {1.0f, 1.0f}, float rotation = 0.0f,
              Transform* parent = nullptr);
    ~Transform() = default;

    Transform* GetParent() const;
    void SetParent(Transform* transform);

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

    Vec2f GetWorldPosition() const;
    Vec2f GetWorldScale() const;
    float GetWorldRotation() const;

    Matrix4 GetMatrix() const;
private:
    Transform* m_parent;
    Matrix4 m_matrix;
};
}  // namespace Cleave