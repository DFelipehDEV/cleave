#pragma once
#include <string>
#include <iostream>

#include "Resource.hpp"

namespace Cleave {
class Texture : public Resource {
public:
    Texture() : m_width(1), m_height(1) {};
    ~Texture() = default;

    std::string GetTypeName() const override { return "cleave::Texture"; }

    uint32_t GetTextureId() const;
    void SetTextureId(int id);

    int GetWidth() const;
    void SetWidth(int width);

    int GetHeight() const;
    void SetHeight(int height);
private:
    uint32_t m_textureId = 0;
    int m_width, m_height;
};

class TextureLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string& extension) const override {
        return extension == ".png" || extension == ".jpg" ||
               extension == ".jpeg" || extension == ".bmp" ||
               extension == ".tga" || extension == ".gif" ||
               extension == ".psd" || extension == ".hdr" ||
               extension == ".pic";
    }
};
}  // namespace Cleave