#pragma once
#include <memory>

#include "GameView.hpp"
#include "entities/Entity.hpp"

namespace Cleave {
namespace Editor {
class Hierarchy {
public:
    Hierarchy(Entity* selectedEntity) : m_selectedEntity(selectedEntity) {}
    ~Hierarchy() = default;

    void OnRender(Scene* scene);

    Entity* GetSelectedEntity();

private:
    Entity* m_selectedEntity;
};
}  // namespace Editor
}  // namespace Cleave