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

