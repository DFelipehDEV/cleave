#pragma once
#include <GL/glew.h>

#include "Resource.hpp"


class Texture : public Resource {
public:
    Texture() : m_width(1), m_height(1) {};
    ~Texture() { Destroy(); }

    std::string GetTypeName() const override { return "cleave::Texture"; }

    bool CreateFromFile(const std::string& path);

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;

    void Destroy();

    int GetWidth() const;

    int GetHeight() const;

private:
    uint32_t m_textureId;
    int m_width, m_height;
};