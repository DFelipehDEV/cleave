#include "resources/ResourceManager.hpp"

#include <algorithm>
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
                    auto resource = loader->Load(entry.path().generic_string(), this);
                    if (resource) {
                        std::string relPath = std::filesystem::relative(entry.path()).generic_string();
                        m_resources[relPath] = resource;
                        LOG_INFO("Loaded resource: " << relPath);
                    }
                    break;
                }
            }
        }
    }
}

std::filesystem::path ResourceManager::GetResourceRoot() const { return m_resourceRoot; }
void ResourceManager::SetResourceRoot(const std::filesystem::path& path) { m_resourceRoot = path; }

Renderer* ResourceManager::GetRenderer() const { return m_renderer; }
void ResourceManager::SetRenderer(Renderer* renderer) { m_renderer = renderer; }

}  // namespace Cleave