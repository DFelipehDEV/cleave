#include "scene/Scene.hpp"

#include "scene/JsonSceneSerializer.hpp"
#include "Scene.hpp"
#include "Log.hpp"
#include "services/ResourceManager.hpp"

namespace Cleave {
std::shared_ptr<Scene> Scene::Instantiate() const {
    return std::dynamic_pointer_cast<Scene>(SceneLoader().Load(GetPath(), GET_RESMGR()));
}

std::unique_ptr<Entity> Scene::ReleaseRoot() { return std::move(m_root); }

Entity* Scene::GetRoot() const { return m_root.get(); }
void Scene::SetRoot(std::unique_ptr<Entity> root) { m_root = std::move(root); }

void Scene::AddSubScene(std::shared_ptr<Scene> subScene) {
    if (!subScene || !subScene->GetRoot()) {
        LOG_ERROR("Failed to load sub scene" << subScene->GetPath());
        return;
    }
    subScene = subScene->Instantiate();

    m_root->AddChild(subScene->ReleaseRoot());
}

void Scene::Clear() { m_root.release(); }

void Scene::Tick() {
    if (m_root) {
        m_root->Tick(16.6666f);
    }
}
void Scene::Render(Renderer* renderer) {
    if (m_root) {
        m_root->Render(renderer);
    }
}

std::shared_ptr<Resource> SceneLoader::Load(const std::string& path, ResourceManager* resourceManager) {
    auto scene = JsonSceneSerializer::Load(path);
    scene->SetPath(path);
    return scene;
}
}  // namespace Cleave