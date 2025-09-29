#pragma once
#include "entities/Entity.hpp"

#include "scene/Scene.hpp"

namespace Cleave {
namespace Editor {
class Properties {
public:
    Properties() = default;
    ~Properties() = default;
    void OnRender(Scene* scene);

    EntityId GetEntityId() const;
    void SetEntityId(EntityId id);

    void Clear();

private:
    EntityId m_entityId = "";
};
}  // namespace Editor
}  // namespace Cleave