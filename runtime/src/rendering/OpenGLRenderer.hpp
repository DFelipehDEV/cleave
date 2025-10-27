#pragma once
#include "rendering/Renderer.hpp"
#include "rendering/RenderTarget.hpp"
#include <unordered_map>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
#ifdef DrawText
#undef DrawText
#endif

#ifdef CreateFont
#undef CreateFont
#endif

namespace Cleave {
class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer()
        : m_projection(Matrix4::Ortho(0.0f, 512.0f, 288.0f, 0.0f, -1.0f, 1.0f)),
          m_viewport(0.0f, 0.0f, 512.0f, 288.0f) {}
    ~OpenGLRenderer();
    void Initialize(Window& window);
    void Terminate();

    void BeginFrame();
    void EndFrame();

    uint32_t GetDrawCalls() const;
    uint32_t GetTextureSwaps() const;

    int GetDepth() const;
    void SetDepth(int depth);

    Matrix4 GetProjection() const;
    void SetProjection(Matrix4 projection);

    Rect4f GetViewPort() const;
    void SetViewPort(Rect4f viewport);

    BlendMode GetBlendMode() const;
    void SetBlendMode(BlendMode mode);

    ShaderHandle CreateShader(const std::string_view vertex, const std::string_view fragment);
    void SetShader(ShaderHandle handle);
    void UseShader(ShaderHandle handle);
    void SetShaderUniformInt(const std::string_view name, int value) const;
    void SetShaderUniformFloat(const std::string_view name, float value) const;
    void SetShaderUniformVector2f(const std::string_view name, float x, float y) const;
    void SetShaderUniformVector3f(const std::string_view name, float x, float y, float z) const;
    void SetShaderUniformVector4f(const std::string_view name, float x, float y, float z, float w) const;
    void SetShaderUniformMatrix4(const std::string_view name, Matrix4 matrix) const;

    void SetTexture(TextureHandle handle);
    void UseTexture(TextureHandle handle);
    Renderer::TextureInfo CreateFallbackTexture();
    Renderer::TextureInfo CreateTexture(const std::string_view path);
    Vec2i GetTextureSize(TextureHandle handle) const;
    
    void SetMaterial(Material material);

    FontHandle CreateFont(const std::string_view path, int size);

    RenderTargetHandle CreateRenderTarget(int width, int height);
    void SetRenderTarget(RenderTargetHandle handle);
    void UseRenderTarget(RenderTargetHandle handle);
    void ClearRenderTarget();

    void AddRenderCommand(std::unique_ptr<RenderCommand> command);

    void ClearColor(Color color);

    void DrawQuad(Rect4f rect,
                  float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White());
    void DrawQuad(Rect4f rect, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f, Color color = Color::White());
    void DrawSprite(Transform transform, Material material);

    void DrawLine(float x1, float y1, float x2, float y2, Color color);
    void DrawRect(Rect4f rect, Color color);
    void DrawRectOutline(Rect4f rect, Color color);
    void DrawCircle(float x, float y, float radius, Color color, int segments = 16);
    void DrawText(const std::string_view text, FontHandle font, float x, float y, float scale, Color color);

    const Glyph* GetGlyph(FontHandle font, char c);
private:
    void ApplyMaterialUniforms(const Material& material) const;
    struct RenderTargetData {
        RenderTarget target;
        GLuint frameBuffer = 0;
    };

    std::unordered_map<ShaderHandle, GLuint> m_shaders;
    std::unordered_map<TextureHandle, GLuint> m_textures;
    std::unordered_map<TextureHandle, TextureInfo> m_textureInfos;
    std::unordered_map<FontHandle, std::unordered_map<char, Glyph>> m_fonts;
    std::unordered_map<RenderTargetHandle, RenderTargetData> m_renderTargets;
    std::vector<std::unique_ptr<RenderCommand>> m_renderCommands;
    int m_depth = 0;
    Matrix4 m_projection;
    Rect4f m_viewport;
    BlendMode m_blendMode = BlendMode::NORMAL;

    uint32_t m_drawCalls = 0;
    uint32_t m_textureSwaps = 0;

    FT_Library m_ftLibrary;

    Material m_currentMaterial;
    TextureHandle m_currentTexture = 0;
    ShaderHandle m_currentShader = 0;
    RenderTargetHandle m_currentRenderTarget = -1;
    GLuint m_quadVAO = 0;
    GLuint m_quadVBO = 0;
    GLuint m_quadEBO = 0;
};
}