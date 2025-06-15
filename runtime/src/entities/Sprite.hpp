#pragma once
#include "Entity.hpp"
#include "resources/Texture.hpp"

class Sprite : public Entity {
public:
    Sprite(Transform transform = Transform(), Texture* texture = nullptr,
           Vec2f origin = {0.5f, 0.5f});
    ~Sprite() = default;

    void Init(
        const std::unordered_map<std::string, Property> properties) override;
    void OnRender(Renderer* renderer) override;

    static const char* GetTypeName() { return "cleave::Sprite"; }

    const std::unordered_map<std::string, Property> GetProperties()
        const override;

    static Entity* Create();

    Texture* GetTexture() const;
    void SetTexture(Texture* texture);

    Vec2f GetOrigin() const;
    void SetOrigin(Vec2f origin);

private:
    Texture* m_texture = nullptr;
    Vec2f m_origin;
};