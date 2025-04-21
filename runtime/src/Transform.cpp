#include "Transform.hpp"


Transform::Transform(const Vec2f position, const Vec2f scale, float rotation, Transform* parent) : m_parent(parent) {
    Translate(position);
    Rotate(rotation);
    Scale(scale);
}

Transform* Transform::GetParent() const { return m_parent; }
void Transform::SetParent(Transform* parent) { m_parent = parent; }

Vec2f Transform::GetWorldPosition() const {
    if (m_parent)
        return m_parent->GetWorldPosition() + GetPosition();
    else
        return GetPosition();
}

Vec2f Transform::GetWorldScale() const {
    if (m_parent)
        return m_parent->GetWorldScale() * GetScale();
    else
        return GetScale();
}

float Transform::GetWorldRotation() const {
    if (m_parent)
        return m_parent->GetWorldRotation() + GetRotation();
    else
        return GetRotation();
}