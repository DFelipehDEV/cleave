#include "resources/ResourceManager.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>

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
                        std::string relPath = std::filesystem::relative(entry.path(), m_resourceRoot).string();
                        std::replace(relPath.begin(), relPath.end(), '\\', '/');
                        m_resources[relPath] = resource;
                        std::cout << "Loaded resource: " << relPath << std::endl;
                    }
                    break;
                }
            }
        }
    }
}

}  // namespace Cleave