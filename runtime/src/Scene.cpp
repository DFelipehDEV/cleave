#include "Scene.hpp"

Scene::~Scene() {
    if (m_root) {
        delete m_root;
    }
}

Entity* Scene::GetRoot() const { return m_root; }
void Scene::SetRoot(Entity* root) {
    if (m_root) {
        delete m_root;
    }
    m_root = root;
}

void Scene::Tick() {
    if (m_root) {
        m_root->OnTick(16.6666f);
    }
}
void Scene::Render() {
    if (m_root) {
        m_root->OnRender(nullptr);
    }
}