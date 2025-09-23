#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Window.hpp"
#include "math/Rect4.hpp"
#include "rendering/Color.hpp"
#include "rendering/TextureFormat.hpp"
#include "rendering/TextureHandle.hpp"
#include "rendering/ShaderHandle.hpp"
#include "rendering/FontHandle.hpp"
#include "math/Matrix4.hpp"
#include "math/Transform.hpp"

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
    virtual void SetShaderUniformInt(const std::string& name, int value) const = 0;
    virtual void SetShaderUniformFloat(const std::string& name, float value) const = 0;
    virtual void SetShaderUniformVector2f(const std::string& name, float x, float y) const = 0;
    virtual void SetShaderUniformVector3f(const std::string& name, float x, float y, float z) const = 0;
    virtual void SetShaderUniformVector4f(const std::string& name, float x, float y, float z, float w) const = 0;
    virtual void SetShaderUniformMatrix4(const std::string& name, const float* matrix) const = 0;

    virtual void SetTexture(TextureHandle handle) = 0;
    virtual void UseTexture(TextureHandle texture) = 0;

    struct TextureInfo {
        TextureHandle handle = 0;
        int width = 0;
        int height = 0;
        TextureFormat format = TextureFormat::RGBA;
    };
    virtual TextureInfo CreateTexture(const std::string& path) = 0;

    virtual ShaderHandle CreateShader(const std::string& vertex, const std::string& fragment) = 0;

    virtual FontHandle CreateFont(const std::string& path, int size = 48) = 0;

    virtual const std::vector<std::unique_ptr<RenderCommand>>& GetRenderCommands() const = 0;
    virtual void AddRenderCommand(std::unique_ptr<RenderCommand> command) = 0;

    virtual void RunRenderCommands() = 0;

    virtual void ClearColor(int r, int g, int b, int a) = 0;

    virtual void DrawQuad(float x, float y, float w, float h, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White()) = 0;
    virtual void DrawQuad(float x, float y, float w, float h, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White()) = 0;
    
    virtual void DrawLine(float x1, float y1, float x2, float y2, Color color) = 0;
    virtual void DrawRect(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawRectOutline(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawCircle(float x, float y, float radius, Color color, int segments = 16) = 0;
    virtual void DrawText(const std::string& text, FontHandle font, float x, float y, float scale, Color color) = 0;

    virtual const Glyph* GetGlyph(FontHandle fontHandle, char c) = 0;
};

struct RenderCommand {
    enum class Type {
        Rect,
        Quad,
        Line,
        Circle,
        Glyph
    } type;
    int depth = 0;
    TextureHandle texture = -1;
    ShaderHandle shader = -1;
    BlendMode blendMode = BlendMode::NORMAL;
    RenderCommand(int _depth, TextureHandle _texture, ShaderHandle _shader) : depth(_depth), texture(_texture), shader(_shader) {}
    virtual ~RenderCommand() = default; 
};

struct RenderQuadCommand : RenderCommand {
    float x, y, w, h, scaleX, scaleY, rotation;
    float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
    Color color = Color::White();

    RenderQuadCommand(float _x, float _y, float _w, float _h,
            float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, 
            TextureHandle _texture = -1, ShaderHandle _shader = -1, 
            int _depth = 0, 
            float _u0 = 0.0f, float _v0 = 0.0f, 
            float _u1 = 1.0f, float _v1 = 1.0f,
            Color _color = Color::White())
            : RenderCommand(_depth, _texture, _shader), x(_x), y(_y), w(_w), h(_h), 
            scaleX(_scaleX), scaleY(_scaleY), rotation(_rotation),
            u0(_u0), v0(_v0), u1(_u1), v1(_v1), color(_color) {
            type = Type::Quad;
        }
};

struct RenderGlyphCommand : public RenderCommand {
    FontHandle font;
    char character;
    float x, y;
    float scale;
    Color color;

    RenderGlyphCommand(FontHandle _font, char _character, float _x, float _y, float _scale, Color _color, ShaderHandle _shader, int _depth)
        : RenderCommand(_depth, -1, _shader), font(_font), character(_character), x(_x), y(_y), scale(_scale), color(_color) {
            type = Type::Glyph;
        }
};


struct RenderLineCommand : RenderCommand {
    float x1, y1, x2, y2;
    Color color;

    RenderLineCommand(float _x1, float _y1, float _x2, float _y2, Color _color, 
                     ShaderHandle _shader = -1, int _depth = 0)
        : RenderCommand(_depth, -1, _shader), x1(_x1), y1(_y1), x2(_x2), y2(_y2), color(_color) {
            type = Type::Line;
        }
};

struct RenderRectCommand : RenderCommand {
    float x, y, w, h;
    Color color;

    RenderRectCommand(float _x, float _y, float _w, float _h, Color _color, 
                     ShaderHandle _shader = -1, int _depth = 0)
        : RenderCommand(_depth, -1, _shader), x(_x), y(_y), w(_w), h(_h), color(_color) {
            type = Type::Rect;
        }
};

struct RenderCircleCommand : RenderCommand {
    float x, y, radius;
    Color color;
    int segments;

    RenderCircleCommand(float _x, float _y, float _radius, Color _color, int _segments = 16, 
                     ShaderHandle _shader = -1, int _depth = 0)
        : RenderCommand(_depth, -1, _shader), x(_x), y(_y), radius(_radius), color(_color), segments(_segments) {
            type = Type::Circle;
        }
};
}  // namespace Cleave