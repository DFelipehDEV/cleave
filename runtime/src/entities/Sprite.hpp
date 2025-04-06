#pragma once
#include "Entity.hpp"
#include "../resources/Texture.hpp"

class Sprite : public Entity {
public:
    Sprite(Vec2f position = {0.0f, 0.0f}, Texture* texture = nullptr, Vec2f origin = {0.5f, 0.5f}, Vec2f scale = {1.0f, 1.0f});
    ~Sprite();

    void OnRender(Renderer* renderer) override;

    Texture* GetTexture() const;
    void SetTexture(Texture* texture);

    Vec2f GetOrigin() const;
    void SetOrigin(Vec2f origin);

    Vec2f GetScale() const;
    void SetScale(Vec2f scale);
private:
    Texture* m_texture = nullptr;
    Vec2f m_origin, m_scale;
};