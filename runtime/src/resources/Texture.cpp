#include "Texture.hpp"

#include <iostream>
#include <algorithm>

#include "thirdparty/stb_image.h"

namespace Cleave {
bool Texture::CreateFromFile(const std::string& path) {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data =
        stbi_load(path.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load texture from file: " << path << std::endl;
        return false;
    }
    std::string pathStr = path;
    #ifdef _WIN32
    std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
    #endif
    SetPath(pathStr);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::Destroy() {
    if (m_textureId) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }
}

int Texture::GetWidth() const { return m_width; }

int Texture::GetHeight() const { return m_height; }

std::shared_ptr<Resource> TextureLoader::Load(const std::string& path) {
    auto texture = std::make_shared<Texture>();

    if (texture->CreateFromFile(path)) {
        return texture;
    }

    // Create missing texture fallback
    GLuint missingTextureID = CreateMissingTexture();
    if (missingTextureID != 0) {
        texture->SetId(missingTextureID);
        texture->SetPath("MissingTexture");
        return texture;
    }

    return nullptr;
}

GLuint TextureLoader::CreateMissingTexture() {
    constexpr int width = 2;
    constexpr int height = 2;
    constexpr uint8_t pixels[width * height * 3] = {
        255, 0, 255, 0,   0, 0,   // Pink, Black
        0,   0, 0,   255, 0, 255  // Black, Pink
    };

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                    GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error while creating missing texture: " << err
                    << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }

    return textureID;
}
}  // namespace Cleave