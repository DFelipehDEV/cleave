#pragma once
#include <memory>

#include "entities/Entity.hpp"

namespace Cleave {
class Scene {
public:
    Scene(std::unique_ptr<Entity> root = nullptr) : m_root(std::move(root)) {}
    ~Scene() = default;

    std::unique_ptr<Entity> ReleaseRoot();
    Entity* GetRoot() const;
    void SetRoot(std::unique_ptr<Entity> root);

    void Clear();

    void Tick();
    void Render(Renderer* renderer);

private:
    std::unique_ptr<Entity> m_root;
};
}  // namespace Cleave