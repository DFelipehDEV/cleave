#pragma once
#include "entities/Entity.hpp"

namespace Cleave {
namespace Editor {
class Properties {
public:
    Properties() = default;
    ~Properties() = default;
    void OnRender();

    Entity* GetEntity() const;
    void SetEntity(Entity* entity);

    void Clear();

private:
    Entity* m_entity;
};
}  // namespace Editor
}  // namespace Cleave