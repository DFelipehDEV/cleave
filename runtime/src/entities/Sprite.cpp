#include "Sprite.hpp"

#include "math/Matrix4.hpp"
#include "services/Services.hpp"
#include "services/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/Material.hpp"
#include "resources/Texture.hpp"

namespace Cleave {
Sprite::Sprite(Transform transform, Material material, Vec2f origin) : Entity(transform), m_material(material), m_origin(origin) {}

const Entity::PropertyMap Sprite::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["origin"] = {GetOrigin().ToString(), Property::Types::Vec2f};
    std::string texturePath;
    if (m_material.texture)
        texturePath = m_material.texture->GetPath();
    else
        texturePath = "";
    properties["texture"] = {texturePath, Entity::Property::Types::String};

    std::string shaderPath;
    if (m_material.shader)
        shaderPath = m_material.shader->GetPath();
    else
        shaderPath = "";
    properties["shader"] = {shaderPath, Entity::Property::Types::String};
    return properties;
}

void Sprite::SetProperty(const std::string_view name, const std::string& value) {
    if (name == "origin") {
        m_origin = Vec2f::FromString(value);
    } else if (name == "texture") {
        if (!value.empty() && GET_RESMGR()->Exists<Texture>(value)) {
            auto tex = GET_RESMGR()->Get<Texture>(value);
            m_material.texture = tex ? tex : nullptr; // Only assign if valid, else clear
            LOG_INFO("Set texture to " << tex->GetHandle());
        } else {
            m_material.texture = nullptr;
        }
    } else if (name == "shader") {
        if (!value.empty() && GET_RESMGR()->Exists<Shader>(value)) {
            auto shader = GET_RESMGR()->Get<Shader>(value);
            m_material.shader = shader ? shader : nullptr; // Only assign if valid, else clear
            LOG_INFO("Set shader to " << shader->GetHandle());
        } else {
            m_material.shader = nullptr;
        }
    } else {
        Entity::SetProperty(name, value);
    }
}

Entity* Sprite::Create() { return new Sprite(); }

Material Sprite::GetMaterial() const { return m_material; }
void Sprite::SetMaterial(Material material) { m_material = material; }

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

void Sprite::OnRender(Renderer* renderer) {
    if (!renderer) return;

    if (m_material.texture != 0) {
        Transform transform = GetTransform().GetGlobalTransform();
        Vec2f globalPosition = transform.GetWorldPosition();
        auto tex = m_material.texture;
        Vec2f offset = Vec2f(tex->GetWidth() * m_origin.x,
                            tex->GetHeight() * m_origin.y);

        auto resourceManager = GET_RESMGR();
        auto shader = resourceManager->Get<Shader>("res/shaders/sprite.vert");

        renderer->SetShader(shader->GetHandle());
        renderer->SetDepth(GetDepth());
        renderer->DrawSprite(transform.GetGlobalTransform(), m_material);
    }
}
}  // namespace Cleave