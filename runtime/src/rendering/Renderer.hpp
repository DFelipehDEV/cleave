#pragma once
#include <string>
#include <memory>

#include "rendering/Color.hpp"
#include "rendering/TextureFormat.hpp"
#include "rendering/TextureHandle.hpp"
#include "rendering/ShaderHandle.hpp"
#include "rendering/FontHandle.hpp"
#include "rendering/RenderTargetHandle.hpp"
#include "rendering/BlendMode.hpp"
#include "rendering/RenderCommand.hpp"
#include "math/Matrix4.hpp"
#include "math/Transform.hpp"
#include "math/Rect4.hpp"

namespace Cleave {
class Window;

struct Glyph {
    TextureHandle texture;
    Vec2i size;       // Size of glyph
    Vec2i bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph

    Glyph() : size(0, 0), bearing(0, 0) {}
    Glyph(TextureHandle tex, Vec2i sz, Vec2i bear, unsigned int adv) 
        : texture(tex), size(sz), bearing(bear), advance(adv) {}
};

class Renderer {
public:
    enum Primitive { Triangle };

    virtual void Initialize(Window& window) = 0;
    virtual void Terminate() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual uint32_t GetDrawCalls() const = 0;
    virtual uint32_t GetTextureSwaps() const = 0;

    virtual int GetDepth() const = 0;
    virtual void SetDepth(int depth) = 0;

    virtual Matrix4 GetProjection() const = 0;
    virtual void SetProjection(Matrix4 projection) = 0;

    virtual Rect4f GetViewPort() const = 0;
    virtual void SetViewPort(Rect4f viewport) = 0;

    virtual BlendMode GetBlendMode() const = 0;
    virtual void SetBlendMode(BlendMode mode) = 0;

    virtual void SetShader(ShaderHandle handle) = 0;
    virtual void UseShader(ShaderHandle shader) = 0;
    virtual void SetShaderUniformInt(const std::string_view name, int value) const = 0;
    virtual void SetShaderUniformFloat(const std::string_view name, float value) const = 0;
    virtual void SetShaderUniformVector2f(const std::string_view name, float x, float y) const = 0;
    virtual void SetShaderUniformVector3f(const std::string_view name, float x, float y, float z) const = 0;
    virtual void SetShaderUniformVector4f(const std::string_view name, float x, float y, float z, float w) const = 0;
    virtual void SetShaderUniformMatrix4(const std::string_view name, Matrix4 matrix) const = 0;

    virtual void SetTexture(TextureHandle handle) = 0;
    virtual void UseTexture(TextureHandle texture) = 0;

    struct TextureInfo {
        TextureHandle handle = 0;
        int width = 0;
        int height = 0;
        TextureFormat format = TextureFormat::RGBA;
    };
    virtual TextureInfo CreateFallbackTexture() = 0;
    virtual TextureInfo CreateTexture(const std::string_view path) = 0;
    virtual Vec2i GetTextureSize(TextureHandle handle) const = 0;

    virtual ShaderHandle CreateShader(const std::string_view vertex, const std::string_view fragment) = 0;

    virtual FontHandle CreateFont(const std::string_view path, int size = 48) = 0;

    virtual RenderTargetHandle CreateRenderTarget(int width, int height) = 0;
    virtual void SetRenderTarget(RenderTargetHandle handle) = 0;
    virtual void UseRenderTarget(RenderTargetHandle handle) = 0;
    virtual void ClearRenderTarget() = 0;

    virtual void AddRenderCommand(std::unique_ptr<RenderCommand> command) = 0;

    virtual void ClearColor(Color color) = 0;

    virtual void DrawQuad(Rect4f rect, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White()) = 0;
    virtual void DrawQuad(Rect4f rect, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White()) = 0;

    virtual void DrawSprite(Transform transform, TextureHandle texture, Color color = Color::White()) = 0;
    
    virtual void DrawLine(float x1, float y1, float x2, float y2, Color color) = 0;
    virtual void DrawRect(Rect4f rect, Color color) = 0;
    virtual void DrawRectOutline(Rect4f rect, Color color) = 0;
    virtual void DrawCircle(float x, float y, float radius, Color color, int segments = 16) = 0;
    virtual void DrawText(const std::string_view text, FontHandle font, float x, float y, float scale, Color color) = 0;

    virtual const Glyph* GetGlyph(FontHandle font, char c) = 0;
};
}  // namespace Cleave