#include "resources/Font.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering/Renderer.hpp"
#include <filesystem>
#include <fstream>

namespace Cleave {
std::shared_ptr<Resource> FontLoader::Load(const std::string& path, ResourceManager* resourceManager) {
    auto fontPath = std::filesystem::path(path);
    
    if (!std::filesystem::exists(fontPath)) {
        std::cout << "Font file does not exist: " << path << std::endl;
        return nullptr;
    }

    auto renderer = resourceManager->GetRenderer();
    if (!renderer) {
        std::cout << "No renderer available" << std::endl;
        return nullptr;
    }

    std::shared_ptr<Font> font = std::make_shared<Font>();
    font->SetPath(path);
    
    int size = 48;
    FontHandle handle = renderer->CreateFont(path, size);
    if (handle == 0) {
        std::cout << "Failed to create font: " << path << std::endl;
        return nullptr;
    }
    
    font->SetHandle(handle);
    font->SetSize(size);
    
    return font;
}
} // namespace Cleave
