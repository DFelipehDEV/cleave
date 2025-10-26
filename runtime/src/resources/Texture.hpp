#pragma once
#include <string>

#include "rendering/TextureFormat.hpp"
#include "Resource.hpp"

namespace Cleave {
typedef uint32_t TextureHandle;
class Texture : public Resource {
public:
    Texture() : m_width(1), m_height(1), m_format(TextureFormat::RGB) {};
    ~Texture() override = default;

    std::string GetTypeName() const override { return "cleave::Texture"; }

    TextureHandle GetHandle() const;
    void SetHandle(TextureHandle handle);

    int GetWidth() const;
    void SetWidth(int width);

    int GetHeight() const;
    void SetHeight(int height);

    TextureFormat GetFormat() const;
    void SetFormat(TextureFormat format);
private:
    TextureHandle m_handle = -1;
    int m_width, m_height;
    TextureFormat m_format;
};

class TextureLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string_view extension) const override {
        return extension == ".png" || extension == ".jpg" ||
               extension == ".jpeg" || extension == ".bmp" ||
               extension == ".tga" || extension == ".gif" ||
               extension == ".psd" || extension == ".hdr" ||
               extension == ".pic";
    }
};
}  // namespace Cleave