#include "services/ResourceManager.hpp"

#include <algorithm>
#include <filesystem>

#include "rendering/Renderer.hpp"

namespace Cleave {

void ResourceManager::RegisterLoader(std::unique_ptr<ResourceLoader> loader) {
    if (loader) {
        m_loaders.push_back(std::move(loader));
    }
}

void ResourceManager::ScanResources(const std::string_view path) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(path)) {
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

Renderer* ResourceManager::GetRenderer() const { return m_renderer; }
void ResourceManager::SetRenderer(Renderer* renderer) { m_renderer = renderer; }

}  // namespace Cleave