#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Window.hpp"
#include "math/Rect4.hpp"
#include "rendering/Color.hpp"
#include "rendering/TextureFormat.hpp"
#include "rendering/TextureHandle.hpp"
#include "rendering/ShaderHandle.hpp"
#include "math/Matrix4.hpp"

namespace Cleave {
enum class BlendMode {
    NORMAL,
    ADD,
    MULTIPLY,
    SUBTRACT,
    SCREEN,
    NONE
};

struct RenderCommand;

class Renderer {
public:
    enum Primitive { Triangle };

    virtual void Initialize(Window& window) = 0;
    virtual void Terminate() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual Matrix4 GetProjection() const = 0;
    virtual void SetProjection(Matrix4 projection) = 0;

    virtual Rect4f GetViewPort() const = 0;
    virtual void SetViewPort(Rect4f viewport) = 0;

    virtual BlendMode GetBlendMode() const = 0;
    virtual void SetBlendMode(BlendMode mode) = 0;

    virtual void UseShader(ShaderHandle shader) = 0;
    virtual void SetShaderUniformInt(const std::string& name, int value) const = 0;
    virtual void SetShaderUniformFloat(const std::string& name, float value) const = 0;
    virtual void SetShaderUniformVector2f(const std::string& name, float x, float y) const = 0;
    virtual void SetShaderUniformVector3f(const std::string& name, float x, float y, float z) const = 0;
    virtual void SetShaderUniformVector4f(const std::string& name, float x, float y, float z, float w) const = 0;
    virtual void SetShaderUniformMatrix4(const std::string& name, const float* matrix) const = 0;

    virtual void UseTexture(TextureHandle texture) = 0;

    struct TextureInfo {
        TextureHandle handle = 0;
        int width = 0;
        int height = 0;
        TextureFormat format = TextureFormat::RGBA;
    };
    virtual TextureInfo CreateTexture(const std::string& path) = 0;
    virtual ShaderHandle CreateShader(const std::string& vertex, const std::string& fragment) = 0;

    virtual const std::vector<RenderCommand*>& GetRenderCommands() const = 0;
    virtual void AddRenderCommand(RenderCommand* command) = 0;

    virtual void RunRenderCommands() = 0;

    virtual void ClearColor(int r, int g, int b, int a) = 0;

    virtual void DrawQuad(float x, float y, float w, float h) = 0;
    virtual void DrawQuad(float x, float y, float w, float h,
                          float u0, float v0, float u1, float v1) = 0;
    virtual void DrawLine(float x1, float y1, float x2, float y2, Color color) = 0;
    virtual void DrawRect(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawRectOutline(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawCircle(float x, float y, float radius, Color color, int segments = 16) = 0;
};

struct RenderCommand {
    int depth = 0;
    RenderCommand(int _depth) : depth(_depth) {}
    virtual void Run(Renderer* renderer) = 0;
};

struct RenderQuadCommand : RenderCommand {
    float x, y, w, h;
    float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
    TextureHandle texture = -1;
    ShaderHandle shader = -1;

    Matrix4 modelMatrix;

    RenderQuadCommand(float _x, float _y, 
                    float _w, float _h, 
                    TextureHandle _texture = -1, 
                    ShaderHandle _shader = -1, 
                    Matrix4 _model = Matrix4(),
                    int _depth = 0, 
                    float _u0 = 0.0f, float _v0 = 0.0f, 
                    float _u1 = 1.0f, float _v1 = 1.0f)
        : RenderCommand(_depth), x(_x), y(_y), w(_w), h(_h), 
          texture(_texture), shader(_shader),
          modelMatrix(_model),
          u0(_u0), v0(_v0), u1(_u1), v1(_v1) {}

    void Run(Renderer* renderer) override {
        if (shader != -1) {
            renderer->UseShader(shader);
            renderer->SetShaderUniformInt("tex", 0);
            renderer->SetShaderUniformMatrix4("projection", (float*)renderer->GetProjection().m);
            renderer->SetShaderUniformMatrix4("model", (float*)modelMatrix.m);
        }

        if (texture != -1) {
            renderer->UseTexture(texture);
        }

        renderer->DrawQuad(x, y, w, h, u0, v0, u1, v1);
    }
};
}  // namespace Cleave