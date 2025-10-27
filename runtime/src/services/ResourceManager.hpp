#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Log.hpp"
#include "resources/Resource.hpp"
#include "services/Services.hpp"

namespace Cleave {
class Renderer;
#define GET_RESMGR() Services::Get<ResourceManager>()
class ResourceManager : public Service {
public:
    void RegisterLoader(std::unique_ptr<ResourceLoader> loader);

    static const char* GetTypeName() { return "cleave::ResourceManager"; }

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

    template <typename T>
    requires std::derived_from<T, Resource>
    bool Exists(const std::string& name) {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
            return true;
        }
        return false;
    }

    template <typename T>
    requires std::derived_from<T, Resource>
    std::vector<std::shared_ptr<T>> GetAll() {
        std::vector<std::shared_ptr<T>> result;
        for (auto& [name, res] : m_resources) {
            if (auto casted = std::dynamic_pointer_cast<T>(res)) {
                result.push_back(casted);
            }
        }
        return result;
    }

    void ScanResources(const std::string_view path = "res");
    void ReloadAll();

    Renderer* GetRenderer() const;
    void SetRenderer(Renderer* renderer);
private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::unique_ptr<ResourceLoader>> m_loaders;
    Renderer* m_renderer;
};

}  // namespace Cleave