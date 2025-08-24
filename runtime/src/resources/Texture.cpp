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
}  // namespace Cleave