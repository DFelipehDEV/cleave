#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

#include "Resource.hpp"

namespace Cleave {
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

class TextureLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path) override;

    bool CanLoad(const std::string& extension) const override {
        return extension == ".png" || extension == ".jpg" ||
               extension == ".jpeg" || extension == ".bmp" ||
               extension == ".tga" || extension == ".gif" ||
               extension == ".psd" || extension == ".hdr" ||
               extension == ".pic";
    }
private:
    GLuint CreateMissingTexture(); 
};
}  // namespace Cleave