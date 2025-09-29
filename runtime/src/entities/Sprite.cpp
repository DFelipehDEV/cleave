#include "Sprite.hpp"

#include "services/Services.hpp"
#include "math/Matrix4.hpp"
#include "services/ResourceManager.hpp"
#include "resources/Shader.hpp"

namespace Cleave {
Sprite::Sprite(Transform transform, std::shared_ptr<Texture> texture,
               Vec2f origin)
    : Entity(transform), m_texture(texture), m_origin(origin) {}

const std::unordered_map<std::string, Entity::Property> Sprite::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["origin"] = {GetOrigin().ToString(), Property::Types::Vec2f};
    std::string texturePath;
    if (m_texture)
        texturePath = GetTexture()->GetPath();
    else
        texturePath = "";
    properties["texture"] = {texturePath, Entity::Property::Types::String};
    return properties;
}

void Sprite::SetProperty(std::string_view name, const std::string& value) {
    if (name == "origin") {
        m_origin = Vec2f::FromString(value);
    } else if (name == "texture") {
        if (!value.empty()) {
            auto tex = GET_RESMGR()->Get<Texture>(value);
            m_texture = tex ? tex : nullptr; // Only assign if valid, else clear
        } else {
            m_texture = nullptr;
        }
    } else {
        Entity::SetProperty(name, value);
    }
}

Entity* Sprite::Create() { return new Sprite(); }

std::shared_ptr<Texture> Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

void Sprite::OnRender(Renderer* renderer) {
    if (!renderer) return;

    if (m_texture) {
        Transform transform = GetTransform().GetGlobalTransform();
        Vec2f globalPosition = transform.GetWorldPosition();
        Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x,
                            m_texture->GetHeight() * m_origin.y);

        auto resourceManager = GET_RESMGR();
        auto shader = resourceManager->Get<Shader>("res/shaders/sprite.vert");

        renderer->SetShader(shader->GetHandle());
        renderer->SetDepth(GetDepth());
        // renderer->DrawQuad(
        //     Rect4f(globalPosition.x - offset.x, globalPosition.y - offset.y, static_cast<float>(m_texture->GetWidth()), static_cast<float>(m_texture->GetHeight())),
        //     GetTransform().GetWorldScale().x, 
        //     GetTransform().GetWorldScale().y, 
        //     GetTransform().GetWorldRotation(),
        //     0.0f, 0.0f, 1.0f, 1.0f, Color::White()
        // );

        renderer->DrawSprite(transform.GetGlobalTransform(), m_texture->GetHandle());
    }

    Entity::OnRender(renderer);
}
}  // namespace Cleave