#pragma once
#include "Entity.hpp"
#include "rendering/Material.hpp"

namespace Cleave {
class Sprite : public Entity {
public:
    Sprite(Transform transform = Transform(),
           Material material = Material(),
           Vec2f origin = {0.5f, 0.5f});

    void OnRender(Renderer* renderer) override;

    static const char* GetTypeName() { return "cleave::Sprite"; }

    const PropertyMap GetProperties() const override;
    void SetProperty(const std::string_view name, const std::string& value) override;

    static Entity* Create();

    Material GetMaterial() const;
    void SetMaterial(Material texture);

    Vec2f GetOrigin() const;
    void SetOrigin(Vec2f origin);

private:
    Material m_material;
    Vec2f m_origin;
};
}  // namespace Cleave