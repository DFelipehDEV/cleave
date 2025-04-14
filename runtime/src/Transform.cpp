#include "Transform.hpp"

Vec2f Transform::GetPosition() const { return m_position; }
void Transform::SetPosition(const Vec2f position) { m_position = position; }

Vec2f Transform::GetScale() const { return m_scale; }
void Transform::SetScale(const Vec2f scale) { m_scale = scale; }

float Transform::GetRotation() const { return m_rotation; }
void Transform::SetRotation(float rotation) { m_rotation = rotation; }

Transform* Transform::GetParent() const { return m_parent; }
void Transform::SetParent(Transform* parent) { m_parent = parent; }

Vec2f Transform::GetWorldPosition() const {
    if (m_parent)
        return m_parent->GetWorldPosition() + m_position;
    else
        return m_position;
}