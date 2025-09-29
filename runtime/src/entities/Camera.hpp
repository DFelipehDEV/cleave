#pragma once

#include "entities/Entity.hpp"

namespace Cleave {
class Camera : public Entity {
public:
    Camera(Transform transform = Transform()) : Entity(transform) {}
    
    float GetZoom() const;
    void SetZoom(float zoom);

    static const char* GetTypeName() { return "cleave::Camera"; }

    const PropertyMap GetProperties() const override;
    void SetProperty(std::string_view name, const std::string& value) override;

    static Entity* Create();
    
    void OnRender(Renderer* renderer) override;
    
private:
    float m_zoom = 1.0f;
};
}  // namespace Cleave