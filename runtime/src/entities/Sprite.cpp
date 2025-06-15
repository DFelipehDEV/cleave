#include "Sprite.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "Services.hpp"
#include "math/Matrix4.hpp"
#include "resources/ResourceManager.hpp"

namespace Cleave {
Sprite::Sprite(Transform transform, Texture* texture, Vec2f origin)
    : Entity(transform), m_texture(texture), m_origin(origin) {}

void Sprite::Init(const std::unordered_map<std::string, Property> properties) {
    Entity::Init(properties);
    if (properties.find("origin") != properties.end())
        m_origin = Vec2f::FromString(properties.at("origin").value);
    if (properties.find("texture") != properties.end())
        m_texture = GET_RESMGR()->textures[properties.at("texture").value];
}

const std::unordered_map<std::string, Entity::Property> Sprite::GetProperties()
    const {
    auto properties = Entity::GetProperties();
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

Texture* Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(Texture* texture) { m_texture = texture; }

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
    resourceManager->shaders["sprite"]->Use();
    resourceManager->shaders["sprite"]->SetUniformInt("tex", 0);
    resourceManager->shaders["sprite"]->SetUniformMatrix4(
        "projection", glm::value_ptr(renderer->GetProjection()));
    resourceManager->shaders["sprite"]->SetUniformMatrix4("model",
                                                          (float*)model.m);

    renderer->DrawTexture(*m_texture, globalPosition.x, globalPosition.y);

    Entity::OnRender(renderer);
}
}  // namespace Cleave