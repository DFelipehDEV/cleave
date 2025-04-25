#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.hpp"
#include "../Rect4.hpp"
#include "../Window.hpp"
#include "../resources/Texture.hpp"

class Renderer {
public:
    enum Primitive {
        Triangle
    };

    virtual void Initialize(Window& window) = 0;
    virtual void Terminate() = 0;

    virtual glm::mat4 GetProjection() const = 0;
    virtual void SetProjection(glm::mat4 projection) = 0;

    virtual Rect4f GetViewPort() const = 0;
    virtual void SetViewPort(Rect4f viewport) = 0;

    virtual void ClearColor(int r, int g, int b, int a) = 0;

    virtual void Draw(Texture* texture, size_t vertexCount, const void* vertexData, const uint32_t* indices, size_t indexCount, Primitive primitive = Primitive::Triangle) = 0;

    virtual void DrawTexture(Texture& texture, float x, float y) = 0;
    virtual void DrawRect(float x, float y, float w, float h, Color color) = 0;
};

class OpenGLRenderer : Renderer {
public:
    OpenGLRenderer::OpenGLRenderer()
    : m_projection(glm::ortho(0.0f, 512.0f, 288.0f, 0.0f, -1.0f, 1.0f)), m_viewport(0.0f, 0.0f, 512.0f, 288.0f) {}
    OpenGLRenderer::~OpenGLRenderer();
    void Initialize(Window& window);
    void Terminate();

    glm::mat4 GetProjection() const;
    void SetProjection(glm::mat4 projection);

    Rect4f GetViewPort() const;
    void SetViewPort(Rect4f viewport);

    void ClearColor(int r, int g, int b, int a);

    void Draw(Texture* texture, size_t vertexCount, const void* vertexData, const uint32_t* indices, size_t indexCount, Primitive primitive = Primitive::Triangle);
    
    void DrawTexture(Texture& texture, float x, float y);
    void DrawRect(float x, float y, float w, float h, Color color);
private:
    glm::mat4 m_projection;
    Rect4f m_viewport;
};