#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "resources/Resource.hpp"
#include "Services.hpp"

#define GET_RESMGR() Services::Get<ResourceManager>("ResMgr")

namespace Cleave {

class ResourceManager {
public:
    void RegisterLoader(std::unique_ptr<ResourceLoader> loader);

    template <typename T>
    std::shared_ptr<T> Get(const std::string& name) {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    void ScanResources(const std::string& path = "resources");
    void ReloadAll();

private:
    std::filesystem::path m_resourceRoot;
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::unique_ptr<ResourceLoader>> m_loaders;
};

}  // namespace Cleave