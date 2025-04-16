#include "Sprite.hpp"

Sprite::Sprite(Transform transform, Texture* texture, Vec2f origin) : Entity(transform), m_texture(texture), m_origin(origin) { }

Texture* Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(Texture* texture) { m_texture = texture; }

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

void Sprite::OnRender(Renderer* renderer) {
    if (!m_texture || !renderer) return;

    Vec2f globalPosition = GetTransform().GetWorldPosition();
    Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x, m_texture->GetHeight() * m_origin.y);
    Vec2f renderPosition = globalPosition - offset;

    renderer->DrawTexture(*m_texture, renderPosition.x, renderPosition.y);

    Entity::OnRender(renderer);
}