#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include "rendering/FontHandle.hpp"
#include "resources/Resource.hpp"
#include "rendering/TextureHandle.hpp"
#include "math/Vec2.hpp"

namespace Cleave {
class Font : public Resource {
public:
    Font() {}
    ~Font() = default;

    std::string GetTypeName() const override { return "cleave::Font"; }
    FontHandle GetHandle() const { return m_handle; }
    void SetHandle(FontHandle handle) { m_handle = handle; }
    
    int GetSize() const { return m_size; }
    void SetSize(int size) { m_size = size; }
private:
    FontHandle m_handle = 0;
    int m_size;
};

class FontLoader : public ResourceLoader {
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string& extension) const override {
        return extension == ".ttf" || extension == ".otf";
    }
};
} // namespace Cleave