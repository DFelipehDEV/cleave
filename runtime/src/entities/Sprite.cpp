#include "Sprite.hpp"

#include "Services.hpp"
#include "math/Matrix4.hpp"
#include "resources/ResourceManager.hpp"
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
        Vec2f globalPosition = GetTransform().GetWorldPosition();
        Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x,
                            m_texture->GetHeight() * m_origin.y);

        Matrix4 model;
        model.Translate(globalPosition);
        model.Translate(-offset);
        model.Rotate(GetTransform().GetWorldRotation());
        model.Scale(GetTransform().GetWorldScale());

        auto resourceManager = GET_RESMGR();
        auto shader = resourceManager->Get<Shader>("res/shaders/sprite.vert");

        renderer->AddRenderCommand(new RenderQuadCommand(
            globalPosition.x,
            globalPosition.y,
            static_cast<float>(m_texture->GetWidth()),
            static_cast<float>(m_texture->GetHeight()), 
            m_texture->GetHandle(), 
            shader->GetHandle(), 
            model, 
            GetDepth()
        ));
    }

    Entity::OnRender(renderer);
}
}  // namespace Cleave