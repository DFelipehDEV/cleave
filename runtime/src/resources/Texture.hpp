#pragma once
#include "Resource.hpp"
#include <GL/glew.h>

class Texture : public Resource {
public:
    unsigned int id = 0;

    Texture() : id(0), m_width(1), m_height(1) {};
    ~Texture() {
        Destroy();
    }

    bool CreateFromFile(const std::string& path);

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;

    void Destroy();

    int GetWidth() const;

    int GetHeight() const;
private:
    int m_width, m_height;
};