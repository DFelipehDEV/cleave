#pragma once
#include <string>
#include <unordered_map>

#include "Shader.hpp"
#include "Texture.hpp"

#define GET_RESMGR() Services::Get<ResourceManager>("ResMgr")

class ResourceManager {
public:
    ResourceManager() {};
    std::unordered_map<std::string, Shader*> shaders;
    std::unordered_map<std::string, Texture*> textures;

    Texture* AddTexture(const std::string& file);
    Shader* AddShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    Shader* AddShaderFromString(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
};