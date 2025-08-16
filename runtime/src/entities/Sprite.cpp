#include "Sprite.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "Services.hpp"
#include "math/Matrix4.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"

namespace Cleave {
Sprite::Sprite(Transform transform, std::shared_ptr<Texture> texture,
               Vec2f origin)
    : Entity(transform), m_texture(texture), m_origin(origin) {}

void Sprite::Init(const std::unordered_map<std::string, Property> properties) {
    Entity::Init(properties);
    if (properties.find("origin") != properties.end())
        m_origin = Vec2f::FromString(properties.at("origin").value);
    if (properties.find("texture") != properties.end())
        m_texture = GET_RESMGR()->Get<Texture>(properties.at("texture").value);
}

const std::unordered_map<std::string, Entity::Property> Sprite::GetProperties()
    const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Entity::Property::Types::Hidden};
    properties["origin"] = {GetOrigin().ToString(),
                            Entity::Property::Types::Vec2f};
    std::string texturePath;
    if (m_texture != nullptr)
        texturePath = GetTexture()->GetPath();
    else
        texturePath = "";
    properties["texture"] = {texturePath, Entity::Property::Types::String};
    return properties;
}

Entity* Sprite::Create() { return new Sprite(); }

std::shared_ptr<Texture> Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(std::shared_ptr<Texture> texture) {
    m_texture = texture;
}

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

void Sprite::OnRender(Renderer* renderer) {
    if (!m_texture || !renderer) return;

    Vec2f globalPosition = GetTransform().GetWorldPosition();
    Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x,
                         m_texture->GetHeight() * m_origin.y);

    Matrix4 model;
    model.Translate(globalPosition);
    model.Translate(-offset);
    model.Rotate(GetTransform().GetWorldRotation());
    model.Scale(GetTransform().GetWorldScale());

    auto resourceManager = GET_RESMGR();
    auto shader = resourceManager->Get<Shader>("sprite");
    shader->Use();
    shader->SetUniformInt("tex", 0);
    shader->SetUniformMatrix4("projection",
                              glm::value_ptr(renderer->GetProjection()));
    shader->SetUniformMatrix4("model", (float*)model.m);

    renderer->DrawTexture(*m_texture, globalPosition.x, globalPosition.y);

    Entity::OnRender(renderer);
}
}  // namespace Cleave