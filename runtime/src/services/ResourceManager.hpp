#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Log.hpp"
#include "resources/Resource.hpp"
#include "services/Services.hpp"

#include "rendering/Renderer.hpp"

namespace Cleave {
#define GET_RESMGR() Services::Get<ResourceManager>()
class ResourceManager : public Service {
public:
    void RegisterLoader(std::unique_ptr<ResourceLoader> loader);

    static std::string GetTypeName() { return "cleave::ResourceManager"; }

    template <typename T>
    requires std::derived_from<T, Resource>
    std::shared_ptr<T> Get(const std::string& name) {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        LOG_ERROR("Resource not found: " << name);
        return nullptr;
    }

    void ScanResources(const std::string& path = "res");
    void ReloadAll();

    std::filesystem::path GetResourceRoot() const;
    void SetResourceRoot(const std::filesystem::path& path);

    Renderer* GetRenderer() const;
    void SetRenderer(Renderer* renderer);
private:
    std::filesystem::path m_resourceRoot;
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::unique_ptr<ResourceLoader>> m_loaders;
    Renderer* m_renderer;
};

}  // namespace Cleave