#include "Shader.hpp"

#include <GL/glew.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "services/ResourceManager.hpp"
#include "rendering/Renderer.hpp"

namespace Cleave {
std::string LoadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("ERROR::SHADER::FILE_NOT_FOUND (" << path << ")");
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

ShaderHandle Shader::GetHandle() const { return m_handle; }
void Shader::SetHandle(ShaderHandle handle) { m_handle = handle; }

std::shared_ptr<Resource> ShaderLoader::Load(const std::string& path, ResourceManager* resourceManager) {
    auto shaderPath = std::filesystem::path(path);
    auto name = shaderPath.stem().string();
    auto dir = shaderPath.parent_path();

    auto vertPath = dir / (name + ".vert");
    auto fragPath = dir / (name + ".frag");

    if (!std::filesystem::exists(vertPath) ||
        !std::filesystem::exists(fragPath)) {
        return nullptr;
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->SetHandle(resourceManager->GetRenderer()->CreateShader(ReadFile(vertPath), ReadFile(fragPath)));
    shader->SetPath(path);

    return shader;
}

std::string ShaderLoader::ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path.string());
    }
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
}  // namespace Cleave