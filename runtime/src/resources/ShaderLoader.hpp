#pragma once
#include <filesystem>
#include <fstream>

#include "Shader.hpp"
#include "resources/Resource.hpp"

namespace Cleave {

class ShaderLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path) override {
        auto shaderPath = std::filesystem::path(path);
        auto name = shaderPath.stem().string();
        auto dir = shaderPath.parent_path();

        auto vertPath = dir / (name + ".vert");
        auto fragPath = dir / (name + ".frag");

        if (!std::filesystem::exists(vertPath) ||
            !std::filesystem::exists(fragPath)) {
            return nullptr;
        }

        return std::make_shared<Shader>(ReadFile(vertPath), ReadFile(fragPath));
    }

    bool CanLoad(const std::string& extension) const override {
        return extension == ".vert" || extension == ".frag";
    }

private:
    std::string ReadFile(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " +
                                     path.string());
        }
        return std::string(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
    }
};

}  // namespace Cleave