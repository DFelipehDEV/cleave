#pragma once

#include "entities/Entity.hpp"

namespace Cleave {
class Camera : public Entity {
public:
    Camera(Transform transform = Transform()) : Entity(transform) {}
    
    float GetZoom() const;
    void SetZoom(float zoom);

    static const char* GetTypeName() { return "cleave::Camera"; }

    const std::unordered_map<std::string, Property> GetProperties() const override;

    static Entity* Create();
    
    void Init(const std::unordered_map<std::string, Property> properties) override;
    void OnRender(Renderer* renderer) override;
    
private:
    float m_zoom = 1.0f;
};
}  // namespace Cleave