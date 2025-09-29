#include "resources/Font.hpp"
#include "services/ResourceManager.hpp"
#include "rendering/Renderer.hpp"
#include <filesystem>
#include <fstream>

namespace Cleave {
std::shared_ptr<Resource> FontLoader::Load(const std::string& path, ResourceManager* resourceManager) {
    auto fontPath = std::filesystem::path(path);

    auto renderer = resourceManager->GetRenderer();
    if (!renderer) {
        LOG_ERROR("No renderer available");
        return nullptr;
    }

    std::shared_ptr<Font> font = std::make_shared<Font>();
    font->SetPath(path);
    
    int size = 48;
    FontHandle handle = renderer->CreateFont(path, size);
    if (handle == 0) {
        LOG_ERROR("Failed to create font: " << path);
        return nullptr;
    }
    
    font->SetHandle(handle);
    font->SetSize(size);
    
    return font;
}
} // namespace Cleave
