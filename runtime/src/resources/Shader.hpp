#pragma once
#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>

#include "Resource.hpp"

namespace Cleave {
class Shader : public Resource {
public:
    Shader() {}
    Shader(const std::string& vertexSource, const std::string& fragmentSource) {
        CreateFromString(vertexSource, fragmentSource);
    }
    ~Shader() override { Destroy(); }

    std::string GetTypeName() const override { return "cleave::Shader"; }

    // Create shader program from file paths
    void CreateFromFile(const std::string& vertexPath,
                        const std::string& fragmentPath);

    // Create shader program from shader source code strings
    void CreateFromString(const std::string& vertexSource,
                          const std::string& fragmentSource);

    // Cleanup resources
    void Destroy();

    // Bind the shader program
    void Use() const;

    // Set uniform values
    void SetUniformInt(const std::string& name, int value) const;
    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformVector2f(const std::string& name, float x, float y) const;
    void SetUniformVector3f(const std::string& name, float x, float y,
                            float z) const;
    void SetUniformVector4f(const std::string& name, float x, float y, float z,
                            float w) const;
    void SetUniformMatrix4(const std::string& name, const float* matrix) const;
};

class ShaderLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path) override;

    bool CanLoad(const std::string& extension) const override {
        return extension == ".vert" || extension == ".frag";
    }

private:
    std::string ReadFile(const std::filesystem::path& path);
};
}  // namespace Cleave