#include "Renderer.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

OpenGLRenderer::~OpenGLRenderer() { Terminate(); }

void OpenGLRenderer::Initialize(Window& window) {
    glfwMakeContextCurrent(window.getGLFWwindow());
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::Terminate() {}

glm::mat4 OpenGLRenderer::GetProjection() const { return m_projection; }
void OpenGLRenderer::SetProjection(glm::mat4 projection) {
    m_projection = projection;
}

Rect4f OpenGLRenderer::GetViewPort() const { return m_viewport; }
void OpenGLRenderer::SetViewPort(Rect4f viewport) {
    m_viewport = viewport;
    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void OpenGLRenderer::ClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void OpenGLRenderer::Draw(Texture* texture, size_t vertexCount,
                          const void* vertexData, const uint32_t* indices,
                          size_t indexCount, Primitive primitive) {
    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * sizeof(float), vertexData,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t),
                 indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (primitive == Primitive::Triangle) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
                       GL_UNSIGNED_INT, 0);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void OpenGLRenderer::DrawTexture(Texture& texture, float x, float y) {
    int w = texture.GetWidth();
    int h = texture.GetHeight();

    float vertices[] = {x,     y + h, 0.0f, 1.0f, x + w, y + h, 1.0f, 1.0f,
                        x + w, y,     1.0f, 0.0f, x,     y,     0.0f, 0.0f};

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    Draw(&texture, 4, vertices, indices, 6, Primitive::Triangle);
}

void OpenGLRenderer::DrawRect(float x, float y, float w, float h, Color color) {
    // TODO: add color
    float vertices[] = {x,     y + h, 0.0f, 1.0f, x + w, y + h, 1.0f, 1.0f,
                        x + w, y,     1.0f, 0.0f, x,     y,     0.0f, 0.0f};

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    Draw(nullptr, 4, vertices, indices, 6, Primitive::Triangle);
}