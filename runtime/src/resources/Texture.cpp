#include "Texture.hpp"

#include <algorithm>

#include "thirdparty/stb_image.h"

#include "services/ResourceManager.hpp"
#include "rendering/Renderer.hpp"

namespace Cleave {
int Texture::GetWidth() const { return m_width; }
void Texture::SetWidth(int width) { m_width = width; }

int Texture::GetHeight() const { return m_height; }
void Texture::SetHeight(int height) { m_height = height; }

TextureFormat Texture::GetFormat() const { return m_format; }
void Texture::SetFormat(TextureFormat format) { m_format = format; }

TextureHandle Texture::GetHandle() const { return m_handle; }
void Texture::SetHandle(TextureHandle handle) { m_handle = handle; }

std::shared_ptr<Resource> TextureLoader::Load(const std::string& path, ResourceManager* resourceManager) {
    auto texture = std::make_shared<Texture>();
    texture->SetPath(path);
    auto textureInfo = resourceManager->GetRenderer()->CreateTexture(path);
    if (textureInfo.handle != 0) {
        texture->SetHandle(textureInfo.handle);
        texture->SetWidth(textureInfo.width);
        texture->SetHeight(textureInfo.height);
        texture->SetFormat(textureInfo.format);
        return texture;
    }

    return nullptr;
}
}  // namespace Cleave