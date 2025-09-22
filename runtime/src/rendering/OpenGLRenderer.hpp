#pragma once
#include "rendering/Renderer.hpp"
#include <unordered_map>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Cleave {
class OpenGLRenderer : Renderer {
public:
    OpenGLRenderer::OpenGLRenderer()
        : m_projection(Matrix4::Ortho(0.0f, 512.0f, 288.0f, 0.0f, -1.0f, 1.0f)),
          m_viewport(0.0f, 0.0f, 512.0f, 288.0f) {}
    OpenGLRenderer::~OpenGLRenderer();
    void Initialize(Window& window);
    void Terminate();

    void BeginFrame();
    void EndFrame();

    Matrix4 GetProjection() const;
    void SetProjection(Matrix4 projection);

    Rect4f GetViewPort() const;
    void SetViewPort(Rect4f viewport);

    BlendMode GetBlendMode() const;
    void SetBlendMode(BlendMode mode);

    void UseShader(ShaderHandle shader);
    void SetShaderUniformInt(const std::string& name, int value) const;
    void SetShaderUniformFloat(const std::string& name, float value) const;
    void SetShaderUniformVector2f(const std::string& name, float x, float y) const;
    void SetShaderUniformVector3f(const std::string& name, float x, float y, float z) const;
    void SetShaderUniformVector4f(const std::string& name, float x, float y, float z, float w) const;
    void SetShaderUniformMatrix4(const std::string& name, const float* matrix) const;

    void UseTexture(TextureHandle handle);

    Renderer::TextureInfo CreateTexture(const std::string& path);
    ShaderHandle CreateShader(const std::string& vertex, const std::string& fragment);
    FontHandle CreateFont(const std::string& fontPath, int fontSize);

    const std::vector<RenderCommand*>& GetRenderCommands() const;
    void AddRenderCommand(RenderCommand* command);

    void RunRenderCommands();

    void ClearColor(int r, int g, int b, int a);

    void DrawQuad(float x, float y, float w, float h);
    void DrawQuad(float x, float y, float w, float h,
                  float u0, float v0, float u1, float v1);
    void DrawLine(float x1, float y1, float x2, float y2, Color color);
    void DrawRect(float x, float y, float w, float h, Color color);
    void DrawRectOutline(float x, float y, float w, float h, Color color);
    void DrawCircle(float x, float y, float radius, Color color, int segments = 16);
    void DrawChar(char c, FontHandle fontHandle, 
                    float x, float y, float scale, Color color);
    void DrawText(const std::string& text, FontHandle fontHandle, 
                    float x, float y, float scale, Color color);

    const Glyph* GetGlyph(FontHandle fontHandle, char c);
private:
    std::unordered_map<ShaderHandle, GLuint> m_shaders;
    std::unordered_map<TextureHandle, GLuint> m_textures;
    std::unordered_map<FontHandle, std::unordered_map<char, Glyph>> m_fonts;
    std::vector<RenderCommand*> m_renderCommands;
    Matrix4 m_projection;
    Rect4f m_viewport;
    BlendMode m_blendMode = BlendMode::NORMAL;

    FT_Library m_ftLibrary;

    GLuint m_currentTexture = 0;
    GLuint m_currentShader = 0;
    GLuint m_quadVAO = 0;
    GLuint m_quadVBO = 0;
    GLuint m_quadEBO = 0;
};
}