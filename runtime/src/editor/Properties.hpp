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

    EntityID GetEntityId() const;
    void SetEntityId(EntityID id);

    void Clear();

private:
    EntityID m_entityId = 0;
};
}  // namespace Editor
}  // namespace Cleave