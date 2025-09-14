#include "Texture.hpp"

#include <iostream>
#include <algorithm>

#include "thirdparty/stb_image.h"

#include "resources/ResourceManager.hpp"

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
    std::string pathStr = path;
    #ifdef _WIN32
    std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
    #endif
    texture->SetPath(pathStr);
    auto textureInfo = resourceManager->GetRenderer()->CreateTexture(pathStr);
    if (textureInfo.handle != 0) {
        texture->SetHandle(textureInfo.handle);
        texture->SetWidth(textureInfo.width);
        texture->SetHeight(textureInfo.height);
        texture->SetFormat(textureInfo.format);
        return texture;
    }

    // // Create missing texture fallback
    // GLuint missingTextureID = CreateMissingTexture();
    // if (missingTextureID != 0) {
    //     texture->SetId(missingTextureID);
    //     texture->SetPath("MissingTexture");
    //     return texture;
    // }

    return nullptr;
}

// GLuint TextureLoader::CreateMissingTexture() {
//     constexpr int width = 2;
//     constexpr int height = 2;
//     constexpr uint8_t pixels[width * height * 3] = {
//         255, 0, 255, 0,   0, 0,   // Pink, Black
//         0,   0, 0,   255, 0, 255  // Black, Pink
//     };

//     GLuint textureID;
//     glGenTextures(1, &textureID);
//     glBindTexture(GL_TEXTURE_2D, textureID);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
//                     GL_UNSIGNED_BYTE, pixels);

//     glBindTexture(GL_TEXTURE_2D, 0);

//     GLenum err = glGetError();
//     if (err != GL_NO_ERROR) {
//         std::cerr << "OpenGL error while creating missing texture: " << err
//                     << std::endl;
//         glDeleteTextures(1, &textureID);
//         return 0;
//     }

//     return textureID;
// }
}  // namespace Cleave