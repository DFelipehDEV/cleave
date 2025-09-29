#pragma once
#include <memory>

#include "entities/Entity.hpp"
#include "resources/Resource.hpp"

namespace Cleave {
class Scene : public Resource {
public:
    Scene(std::unique_ptr<Entity> root = nullptr) : m_root(std::move(root)) {}
    ~Scene() = default;

    std::string GetTypeName() const override { return "cleave::Scene"; }

    std::shared_ptr<Scene> Instantiate() const;

    std::unique_ptr<Entity> ReleaseRoot();
    Entity* GetRoot() const;
    void SetRoot(std::unique_ptr<Entity> root);

    void AddSubScene(std::shared_ptr<Scene> subScene);

    void ReassignEntityIDs();

    void Clear();

    void Tick();
    void Render(Renderer* renderer);

private:
    std::unique_ptr<Entity> m_root;
};

class SceneLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;
    bool CanLoad(const std::string& extension) const override {
        return extension == ".jscn";
    }
};
}  // namespace Cleave