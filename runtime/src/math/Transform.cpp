#define _USE_MATH_DEFINES
#include "Transform.hpp"

#include <cmath>

namespace Cleave {
Transform::Transform(const Vec2f position, const Vec2f scale, float rotation,
                     Transform* parent)
    : m_parent(parent) {
    m_matrix = Matrix4::Identity();
    Translate(position);
    Rotate(rotation);
    Scale(scale);
}

Transform* Transform::GetParent() const { return m_parent; }
void Transform::SetParent(Transform* parent) { m_parent = parent; }

void Transform::Translate(Vec2f translation) {
    m_matrix.m[3][0] += translation.x;
    m_matrix.m[3][1] += translation.y;
}

void Transform::Scale(Vec2f scale) {
    Matrix4 scaling;
    scaling.m[0][0] = scale.x;
    scaling.m[1][1] = scale.y;
    m_matrix = m_matrix * scaling;
}

void Transform::Rotate(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);
    Matrix4 rotation;
    rotation.m[0][0] = c;
    rotation.m[0][1] = -s;
    rotation.m[1][0] = s;
    rotation.m[1][1] = c;
    m_matrix = m_matrix * rotation;
}

Vec2f Transform::GetPosition() const { return Vec2f(m_matrix.m[3][0], m_matrix.m[3][1]); }
void Transform::SetPosition(Vec2f position) {
    m_matrix.m[3][0] = position.x;
    m_matrix.m[3][1] = position.y;
}

Vec2f Transform::GetScale() const {
    return Vec2f(
        std::sqrt(m_matrix.m[0][0] * m_matrix.m[0][0] + m_matrix.m[1][0] * m_matrix.m[1][0]),
        std::sqrt(m_matrix.m[0][1] * m_matrix.m[0][1] + m_matrix.m[1][1] * m_matrix.m[1][1])
    );
}

void Transform::SetScale(Vec2f scale) {
    float currentScaleX = std::sqrt(m_matrix.m[0][0] * m_matrix.m[0][0] + m_matrix.m[1][0] * m_matrix.m[1][0]);
    float currentScaleY = std::sqrt(m_matrix.m[0][1] * m_matrix.m[0][1] + m_matrix.m[1][1] * m_matrix.m[1][1]);
    if (currentScaleX != 0.0f) {
        m_matrix.m[0][0] /= currentScaleX;
        m_matrix.m[1][0] /= currentScaleX;
    }
    if (currentScaleY != 0.0f) {
        m_matrix.m[0][1] /= currentScaleY;
        m_matrix.m[1][1] /= currentScaleY;
    }

    m_matrix.m[0][0] *= scale.x;
    m_matrix.m[1][0] *= scale.x;
    m_matrix.m[0][1] *= scale.y;
    m_matrix.m[1][1] *= scale.y;
}

float Transform::GetRotation() const { return std::atan2(m_matrix.m[1][0], m_matrix.m[0][0]); }

float Transform::GetRotationDegrees() const {
    return GetRotation() * (180.0f / M_PI);
}

void Transform::SetRotation(float radians) {
    Vec2f scale = GetScale();
    float c = std::cos(radians);
    float s = std::sin(radians);

    m_matrix.m[0][0] = c * scale.x;
    m_matrix.m[0][1] = -s * scale.y;
    m_matrix.m[1][0] = s * scale.x;
    m_matrix.m[1][1] = c * scale.y;
}

void Transform::SetRotationDegrees(float degrees) {
    SetRotation(degrees * (M_PI / 180.0f));
}

Transform Transform::GetGlobalTransform() const {
    Vec2f worldPos = GetWorldPosition();
    Vec2f worldScale = GetWorldScale();
    float worldRot = GetWorldRotation();
    return Transform(worldPos, worldScale, worldRot);
}

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

const Matrix4& Transform::GetMatrix() const { return m_matrix; }
}  // namespace Cleave