#include "Sprite.hpp"

Sprite::Sprite(Vec2f position, Texture* texture, Vec2f origin, Vec2f scale) : Entity(position), m_texture(texture), m_origin(origin), m_scale(scale) { }
Sprite::~Sprite() { }

Texture* Sprite::GetTexture() const { return m_texture; }
void Sprite::SetTexture(Texture* texture) { m_texture = texture; }

Vec2f Sprite::GetOrigin() const { return m_origin; }
void Sprite::SetOrigin(Vec2f origin) { m_origin = origin; }

Vec2f Sprite::GetScale() const { return m_scale;}
void Sprite::SetScale(Vec2f scale) { m_scale = scale; }

void Sprite::OnRender(Renderer* renderer) {
    if (!m_texture || !renderer) return;

    Vec2f globalPosition = GetGlobalPosition();
    Vec2f offset = Vec2f(m_texture->GetWidth() * m_origin.x, m_texture->GetHeight() * m_origin.y);
    Vec2f renderPosition = globalPosition - offset;

    renderer->DrawTexture(*m_texture, renderPosition.x, renderPosition.y);
}