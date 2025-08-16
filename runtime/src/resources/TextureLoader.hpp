#pragma once
#include <iostream>
#include <memory>

#include "Texture.hpp"
#include "resources/Resource.hpp"

namespace Cleave {

class TextureLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path) override {
        auto texture = std::make_shared<Texture>();

        if (texture->CreateFromFile(path)) {
            texture->SetPath(path);
            return texture;
        }

        // Create missing texture fallback
        GLuint missingTextureID = CreateMissingTexture();
        if (missingTextureID != 0) {
            texture->SetId(missingTextureID);
            texture->SetPath("/MissingTexture");
            return texture;
        }

        return nullptr;
    }

    bool CanLoad(const std::string& extension) const override {
        return extension == ".png" || extension == ".jpg";
    }

    GLuint CreateMissingTexture() {
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
};

}  // namespace Cleave