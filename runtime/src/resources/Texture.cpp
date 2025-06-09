#include "Texture.hpp"
#include "thirdparty/stb_image.h"
#include <iostream>

bool Texture::CreateFromFile(const std::string& path) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load texture from file: " << path << std::endl;
        return false;
    }
    SetPath(path);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Destroy()  {
    if (m_id) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

int Texture::GetWidth() const {
    return m_width;
}

int Texture::GetHeight() const {
    return m_height;
}