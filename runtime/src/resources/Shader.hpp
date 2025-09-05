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
    ~Shader() = default;

    std::string GetTypeName() const override { return "cleave::Shader"; }

    int GetShaderId() const;
    void SetShaderId(int id);
private:
    int m_shaderId = 0;
};

class ShaderLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string& extension) const override {
        return extension == ".vert" || extension == ".frag";
    }
private:
    std::string ReadFile(const std::filesystem::path& path);
};
}  // namespace Cleave