#include "Camera.hpp"

namespace Cleave {

float Camera::GetZoom() const { return m_zoom; }
void Camera::SetZoom(float zoom) { m_zoom = zoom; }

const Entity::PropertyMap Camera::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Entity::Property::Types::Hidden};
    properties["zoom"] = {std::to_string(m_zoom), Entity::Property::Types::Float};
    return properties;
}
void Camera::SetProperty(const std::string_view name, const std::string& value) {
    if (name == "zoom") {
        m_zoom = std::stof(value);
    } else {
        Entity::SetProperty(name, value);
    }
}

Entity* Camera::Create() { return new Camera(); }

void Camera::OnRender(Renderer* renderer) {
    float aspect = renderer->GetViewPort().w / renderer->GetViewPort().h;
    Matrix4 projection = Matrix4::Ortho(
        -aspect * m_zoom, aspect * m_zoom,
        -m_zoom, m_zoom,
        -1.0f, 1.0f);
    renderer->SetProjection(projection);
}

}  // namespace Cleave