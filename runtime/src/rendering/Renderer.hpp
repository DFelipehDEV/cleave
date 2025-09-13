#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Window.hpp"
#include "math/Rect4.hpp"
#include "rendering/Color.hpp"
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

    virtual int GetSpriteShader() const = 0;
    virtual void SetSpriteShader(int shaderId) = 0;

    virtual void UseShader(int shaderId) = 0;
    virtual void SetShaderUniformInt(const std::string& name, int value) const = 0;
    virtual void SetShaderUniformFloat(const std::string& name, float value) const = 0;
    virtual void SetShaderUniformVector2f(const std::string& name, float x, float y) const = 0;
    virtual void SetShaderUniformVector3f(const std::string& name, float x, float y, float z) const = 0;
    virtual void SetShaderUniformVector4f(const std::string& name, float x, float y, float z, float w) const = 0;
    virtual void SetShaderUniformMatrix4(const std::string& name, const float* matrix) const = 0;

    virtual void UseTexture(int textureId) = 0;

    struct TextureInfo {
        uint32_t id = 0;
        int width = 0;
        int height = 0;
    };
    virtual TextureInfo CreateTexture(const std::string& path) = 0;
    virtual int CreateShader(const std::string& vertex, const std::string& fragment) = 0;

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
    int textureId = -1;
    int shaderId = -1;

    Matrix4 modelMatrix;
    Matrix4 projectionMatrix;

    RenderQuadCommand(float _x, float _y, float _w, float _h, 
                     int _textureId = -1, int _shaderId = -1, 
                     Matrix4 _model = Matrix4(), 
                     Matrix4 _projection = Matrix4(), 
                     int _depth = 0, float _u0 = 0.0f, float _v0 = 0.0f, float _u1 = 1.0f, float _v1 = 1.0f)
        : RenderCommand(_depth), x(_x), y(_y), w(_w), h(_h), 
          textureId(_textureId), shaderId(_shaderId),
          modelMatrix(_model), projectionMatrix(_projection),
          u0(_u0), v0(_v0), u1(_u1), v1(_v1) {}

    void Run(Renderer* renderer) override {
        if (shaderId != -1) {
            renderer->UseShader(shaderId);
            renderer->SetShaderUniformInt("tex", 0);
            renderer->SetShaderUniformMatrix4("projection", (float*)projectionMatrix.m);
            renderer->SetShaderUniformMatrix4("model", (float*)modelMatrix.m);
        }
        if (textureId != -1) {
            renderer->UseTexture(textureId);
        }
        renderer->DrawQuad(x, y, w, h, u0, v0, u1, v1);
    }
};

struct RenderSpriteCommand : RenderCommand {
    float x, y, w, h;
    int textureId = -1;

    RenderSpriteCommand(float _x, float _y, float _w, float _h, int _textureId = -1, int _depth = 0)
        : RenderCommand(_depth), x(_x), y(_y), w(_w), h(_h), textureId(_textureId) {}

    void Run(Renderer* renderer) override {
        if (textureId != -1) {
            renderer->UseTexture(textureId);
        }
        renderer->UseShader(renderer->GetSpriteShader());
        renderer->DrawQuad(x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
    }
};
}  // namespace Cleave