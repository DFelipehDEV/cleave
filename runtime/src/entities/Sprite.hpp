#pragma once
#include "Entity.hpp"
#include "../resources/Texture.hpp"

// @prop origin : Vec2f
class Sprite : public Entity {
public:
    Sprite(Transform transform = Transform(), Texture* texture = nullptr, Vec2f origin = {0.5f, 0.5f});
    ~Sprite() = default;

    void OnRender(Renderer* renderer) override;

    std::string GetType() const override { return "cleave::Sprite"; }

    Texture* GetTexture() const;
    void SetTexture(Texture* texture);

    Vec2f GetOrigin() const;
    void SetOrigin(Vec2f origin);
private:
    Texture* m_texture = nullptr;
    Vec2f m_origin;
};