#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>

#include "Resource.hpp"
#include "rendering/ShaderHandle.hpp"

namespace Cleave {
class Shader : public Resource {
public:
    Shader() {}
    ~Shader() = default;

    std::string_view GetTypeName() const override { return "cleave::Shader"; }

    ShaderHandle GetHandle() const;
    void SetHandle(ShaderHandle handle);
private:
    ShaderHandle m_handle = 0;
};

class ShaderLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string_view extension) const override {
        return extension == ".vert" || extension == ".frag";
    }
private:
    std::string ReadFile(const std::filesystem::path& path);
};
}  // namespace Cleave