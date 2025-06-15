#include "scene/Scene.hpp"

namespace Cleave {
std::unique_ptr<Entity> Scene::ReleaseRoot() { return std::move(m_root); }

Entity* Scene::GetRoot() const { return m_root.get(); }
void Scene::SetRoot(std::unique_ptr<Entity> root) { m_root = std::move(root); }

void Scene::Clear() { m_root.release(); }

void Scene::Tick() {
    if (m_root) {
        m_root->OnTick(16.6666f);
    }
}
void Scene::Render(Renderer* renderer) {
    if (m_root) {
        m_root->OnRender(renderer);
    }
}
}  // namespace Cleave