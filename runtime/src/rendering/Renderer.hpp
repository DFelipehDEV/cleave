#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Window.hpp"
#include "../resources/Texture.hpp"

class Renderer {
public:
    enum Primitive {
        Triangle
    };

    virtual void Initialize(Window& window) = 0;
    virtual void Terminate() = 0;

    virtual void ClearColor(int r, int g, int b, int a) = 0;

    virtual void Draw(Texture* texture, size_t vertexCount, const void* vertexData, const uint32_t* indices, size_t indexCount, Primitive primitive = Primitive::Triangle) = 0;

    virtual void DrawTexture(Texture& texture, float x, float y) = 0;
    //virtual void DrawRect(float x, float y, float width, float height) = 0;
};

class OpenGLRenderer : Renderer {
public:
    OpenGLRenderer::OpenGLRenderer()
    : m_projection(glm::ortho(0.0f, 512.0f, 288.0f, 0.0f, -1.0f, 1.0f)) {}
    OpenGLRenderer::~OpenGLRenderer();
    void Initialize(Window& window);
    void Terminate();

    void ClearColor(int r, int g, int b, int a);

    void Draw(Texture* texture, size_t vertexCount, const void* vertexData, const uint32_t* indices, size_t indexCount, Primitive primitive = Primitive::Triangle);
    
    void DrawTexture(Texture& texture, float x, float y);
    //void DrawRect(float x, float y, float width, float height);
public:
    glm::mat4 m_projection;
};