#include "Camera.hpp"

namespace Cleave {

float Camera::GetZoom() const { return m_zoom; }
void Camera::SetZoom(float zoom) { m_zoom = zoom; }

void Camera::Init(const std::unordered_map<std::string, Property> properties) {
    Entity::Init(properties);
    if (properties.find("zoom") != properties.end())
        m_zoom = std::stof(properties.at("zoom").value);
}

const std::unordered_map<std::string, Entity::Property> Camera::GetProperties()
    const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Entity::Property::Types::Hidden};
    properties["zoom"] = {std::to_string(m_zoom), Entity::Property::Types::Float};
    return properties;
}

Entity* Camera::Create() { return new Camera(); }

void Camera::OnRender(Renderer* renderer) {
    float aspect = renderer->GetViewPort().width / renderer->GetViewPort().height;
    glm::mat4 projection = glm::ortho(
        -aspect * m_zoom, aspect * m_zoom,
        -m_zoom, m_zoom,
        -1.0f, 1.0f);
    renderer->SetProjection(projection);
}

}  // namespace Cleave