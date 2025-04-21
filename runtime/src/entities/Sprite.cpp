#include "Sprite.hpp"
#include "Services.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "../Matrix4.hpp"

Sprite::Sprite(Transform transform, Texture* texture, Vec2f origin) : Entity(transform), m_texture(texture), m_origin(origin) { }

Texture* Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(Texture* texture) { m_texture = texture; }

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

void Sprite::OnRender(Renderer* renderer) {
    if (!m_texture || !renderer) return;

    Vec2f globalPosition = GetTransform().GetWorldPosition();
    Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x, m_texture->GetHeight() * m_origin.y);

    Matrix4 model;
    model.Translate(globalPosition);
    model.Translate(-offset);
    model.Rotate(GetTransform().GetRotation());
    model.Scale(GetTransform().GetScale());

    Services::GetResourceManager()->shaders["sprite"]->Use();
    Services::GetResourceManager()->shaders["sprite"]->SetUniformInt("tex", 0);
    Services::GetResourceManager()->shaders["sprite"]->SetUniformMatrix4("projection", glm::value_ptr(((OpenGLRenderer*)renderer)->m_projection));
    Services::GetResourceManager()->shaders["sprite"]->SetUniformMatrix4("model", (float*)model.m);

    renderer->DrawTexture(*m_texture, globalPosition.x, globalPosition.y);

    Entity::OnRender(renderer);
}
