#include "ResourceManager.hpp"

#include <iostream>

#include "ResourceManager.hpp"
#include "Texture.hpp" // Assuming Texture wraps an OpenGL texture ID

#include <iostream>
#include <GL/glew.h> // For OpenGL functions

GLuint CreateMissingTexture() {
    // Define a simple 2x2 checkerboard pattern as the missing texture
    constexpr int width = 2;
    constexpr int height = 2;
    constexpr uint8_t pixels[width * height * 3] = {
        255, 0, 255,   0, 0, 0,  // Pink, Black
        0, 0, 0,   255, 0, 255   // Black, Pink
    };

    GLuint textureID;
    glGenTextures(1, &textureID); // Generate a new texture ID
    glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload the pixel data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Check for OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error while creating missing texture: " << err << std::endl;
        glDeleteTextures(1, &textureID); // Clean up the texture if there was an error
        return 0;
    }

    return textureID;
}

Texture* ResourceManager::AddTexture(const std::string& file) {
    Texture* texture = new Texture();
    if (texture->CreateFromFile(file)) {
        textures[file] = texture;
        textures[file]->SetName(file);
    } else {
        std::cout << "Failed to load image: " << file.c_str() << std::endl;

        // Create a default missing texture to avoid crashes
        GLuint missingTextureID = CreateMissingTexture();
        if (missingTextureID != 0) {
            texture->id = missingTextureID;
            texture->SetName("MissingTexture");
            textures[file] = texture;
        } else {
            std::cerr << "Failed to create missing texture for file: " << file.c_str() << std::endl;
            delete texture;
            texture = nullptr;
        }
    }
    return texture;
}

Shader* ResourceManager::AddShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    Shader* shader = new Shader();
    shader->CreateFromFile(vertexPath, fragmentPath);
    shader->SetName(name);
    shaders[name] = shader;
    return shader;
}

Shader* ResourceManager::AddShaderFromString(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
    Shader* shader = new Shader();
    shader->CreateFromString(vertexSource, fragmentSource);
    shader->SetName(name);
    shaders[name] = shader;
    return shader;
}
