#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Window.hpp"
#include "math/Rect4.hpp"
#include "rendering/Color.hpp"

namespace Cleave {
enum class BlendMode {
    NORMAL,
    ADD,
    MULTIPLY,
    SUBTRACT,
    SCREEN,
    NONE
};
class Renderer {
public:
    enum Primitive { Triangle };

    virtual void Initialize(Window& window) = 0;
    virtual void Terminate() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame(Window& window) = 0;

    virtual glm::mat4 GetProjection() const = 0;
    virtual void SetProjection(glm::mat4 projection) = 0;

    virtual Rect4f GetViewPort() const = 0;
    virtual void SetViewPort(Rect4f viewport) = 0;

    virtual BlendMode GetBlendMode() const = 0;
    virtual void SetBlendMode(BlendMode mode) = 0;

    virtual void ClearColor(int r, int g, int b, int a) = 0;

    // virtual void Draw(Texture* texture, size_t vertexCount,
    //                   const void* vertexData, const uint32_t* indices,
    //                   size_t indexCount,
    //                   Primitive primitive = Primitive::Triangle) = 0;

    virtual void DrawQuad(float x, float y, float w, float h) = 0;
    virtual void DrawLine(float x1, float y1, float x2, float y2, Color color) = 0;
    virtual void DrawRect(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawRectOutline(float x, float y, float w, float h, Color color) = 0;
    virtual void DrawCircle(float x, float y, float radius, Color color, int segments = 16) = 0;
};

class OpenGLRenderer : Renderer {
public:
    OpenGLRenderer::OpenGLRenderer()
        : m_projection(glm::ortho(0.0f, 512.0f, 288.0f, 0.0f, -1.0f, 1.0f)),
          m_viewport(0.0f, 0.0f, 512.0f, 288.0f) {}
    OpenGLRenderer::~OpenGLRenderer();
    void Initialize(Window& window);
    void Terminate();

    void BeginFrame();
    void EndFrame(Window& window);

    glm::mat4 GetProjection() const;
    void SetProjection(glm::mat4 projection);

    Rect4f GetViewPort() const;
    void SetViewPort(Rect4f viewport);

    BlendMode GetBlendMode() const;
    void SetBlendMode(BlendMode mode);

    void ClearColor(int r, int g, int b, int a);

    // void Draw(Texture* texture, size_t vertexCount, const void* vertexData,
    //           const uint32_t* indices, size_t indexCount,
    //           Primitive primitive = Primitive::Triangle);

    void DrawQuad(float x, float y, float w, float h);
    void DrawLine(float x1, float y1, float x2, float y2, Color color);
    void DrawRect(float x, float y, float w, float h, Color color);
    void DrawRectOutline(float x, float y, float w, float h, Color color);
    void DrawCircle(float x, float y, float radius, Color color, int segments = 16);

private:
    glm::mat4 m_projection;
    Rect4f m_viewport;
    BlendMode m_blendMode = BlendMode::NORMAL;
    
    GLuint m_quadVAO = 0;
    GLuint m_quadVBO = 0;
    GLuint m_quadEBO = 0;
};
}  // namespace Cleave