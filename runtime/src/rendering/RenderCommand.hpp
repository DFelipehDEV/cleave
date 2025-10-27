#pragma once
#include "rendering/TextureHandle.hpp"
#include "rendering/ShaderHandle.hpp"
#include "rendering/RenderTargetHandle.hpp"
#include "rendering/FontHandle.hpp"
#include "rendering/BlendMode.hpp"
#include "rendering/Color.hpp"
#include "math/Rect4.hpp"
namespace Cleave {
struct RenderCommand {
    enum class Type {
        Rect,
        Quad,
        Line,
        Circle,
        Glyph
    } type;
    int depth = 0;
    TextureHandle texture = 0;
    ShaderHandle shader = 0;
    RenderTargetHandle renderTarget = 0;
    BlendMode blendMode = BlendMode::NORMAL;
    RenderCommand(int _depth, TextureHandle _texture, ShaderHandle _shader, RenderTargetHandle _renderTarget) : depth(_depth), texture(_texture), shader(_shader), renderTarget(_renderTarget) {}
};

struct RenderQuadCommand : RenderCommand {
    Rect4f rect;
    float scaleX, scaleY, rotation;
    float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
    Color color = Color::White();

    RenderQuadCommand(Rect4f _rect,
            float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, 
            TextureHandle _texture = -1, ShaderHandle _shader = -1, 
            int _depth = 0, 
            float _u0 = 0.0f, float _v0 = 0.0f, 
            float _u1 = 1.0f, float _v1 = 1.0f,
            Color _color = Color::White(),
            RenderTargetHandle _renderTarget = 0
        )
            : RenderCommand(_depth, _texture, _shader, _renderTarget), rect(_rect), 
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

    RenderGlyphCommand(FontHandle _font, char _character, float _x, float _y, float _scale, Color _color, ShaderHandle _shader, int _depth, RenderTargetHandle _renderTarget = 0)
        : RenderCommand(_depth, -1, _shader, _renderTarget), font(_font), character(_character), x(_x), y(_y), scale(_scale), color(_color) {
            type = Type::Glyph;
        }
};


struct RenderLineCommand : RenderCommand {
    float x1, y1, x2, y2;
    Color color;

    RenderLineCommand(float _x1, float _y1, float _x2, float _y2, Color _color, 
                     ShaderHandle _shader = -1, int _depth = 0, RenderTargetHandle _renderTarget = 0)
        : RenderCommand(_depth, -1, _shader, _renderTarget), x1(_x1), y1(_y1), x2(_x2), y2(_y2), color(_color) {
            type = Type::Line;
        }
};

struct RenderRectCommand : RenderCommand {
    Rect4f rect;
    Color color;

    RenderRectCommand(Rect4f _rect, Color _color, 
                     ShaderHandle _shader = -1, int _depth = 0, RenderTargetHandle _renderTarget = 0)
        : RenderCommand(_depth, -1, _shader, _renderTarget), rect(_rect), color(_color) {
            type = Type::Rect;
        }
};

struct RenderCircleCommand : RenderCommand {
    float x, y, radius;
    Color color;
    int segments;

    RenderCircleCommand(float _x, float _y, float _radius, Color _color, int _segments = 16, 
                     ShaderHandle _shader = -1, int _depth = 0, RenderTargetHandle _renderTarget = 0)
        : RenderCommand(_depth, -1, _shader, _renderTarget), x(_x), y(_y), radius(_radius), color(_color), segments(_segments) {
            type = Type::Circle;
        }
};
} // namespace Cleave