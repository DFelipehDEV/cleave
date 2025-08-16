#include "resources/ResourceManager.hpp"

#include <filesystem>

namespace Cleave {

void ResourceManager::RegisterLoader(std::unique_ptr<ResourceLoader> loader) {
    if (loader) {
        m_loaders.push_back(std::move(loader));
    }
}

void ResourceManager::ScanResources(const std::string& path) {
    m_resourceRoot = path;

    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(m_resourceRoot)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();

            for (const auto& loader : m_loaders) {
                if (loader->CanLoad(extension)) {
                    auto resource = loader->Load(entry.path().string());
                    if (resource) {
                        std::string name = entry.path().stem().string();
                        m_resources[name] = resource;
                    }
                    break;
                }
            }
        }
    }
}

}  // namespace Cleave