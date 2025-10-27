#pragma once
#include <string>
#include <unordered_map>

#include "resources/Shader.hpp"
#include "resources/Texture.hpp"
#include "rendering/BlendMode.hpp"
#include "math/Vec2.hpp"
#include "math/Matrix4.hpp"

namespace Cleave {
struct Material {
    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Texture> texture = nullptr;
    BlendMode blendMode = BlendMode::NORMAL;
    std::unordered_map<std::string, int> intUniforms;
    std::unordered_map<std::string, float> floatUniforms;
    std::unordered_map<std::string, Vec2f> vec2fUniforms;
    std::unordered_map<std::string, Matrix4> matrix4Uniforms;

    void SetInt(const std::string& name, int value) { intUniforms[name] = value; }
    void SetFloat(const std::string& name, float value) { floatUniforms[name] = value; }
    void SetVec2f(const std::string& name, Vec2f value) { vec2fUniforms[name] = value; }
    void SetMatrix4(const std::string& name, Matrix4 value) { matrix4Uniforms[name] = value; }
};
} // namespace Cleave