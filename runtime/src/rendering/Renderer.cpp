#include "Renderer.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Cleave {
OpenGLRenderer::~OpenGLRenderer() { Terminate(); }

void OpenGLRenderer::Initialize(Window& window) {
    glfwMakeContextCurrent(window.getGLFWwindow());
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertices[] = {
        // x, y, u, v
        0.0f, 1.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 1.0f, // top-right
        1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.0f, 0.0f, 0.0f, 0.0f  // bottom-left
    };

    uint32_t indices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadEBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // unbind VAO
}

void OpenGLRenderer::Terminate() {}

void OpenGLRenderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::EndFrame(Window& window) {
    glfwSwapBuffers(window.getGLFWwindow());
}

glm::mat4 OpenGLRenderer::GetProjection() const { return m_projection; }
void OpenGLRenderer::SetProjection(glm::mat4 projection) {
    m_projection = projection;
}

Rect4f OpenGLRenderer::GetViewPort() const { return m_viewport; }
void OpenGLRenderer::SetViewPort(Rect4f viewport) {
    m_viewport = viewport;
    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

BlendMode OpenGLRenderer::GetBlendMode() const {
    return m_blendMode;
}
void OpenGLRenderer::SetBlendMode(BlendMode mode) {
    if (m_blendMode == mode) return;
    m_blendMode = mode;
    
    switch (mode) {
        case BlendMode::NONE:
            glDisable(GL_BLEND);
            break;
            
        case BlendMode::NORMAL:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
            
        case BlendMode::ADD:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
            
        case BlendMode::MULTIPLY:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
            
        case BlendMode::SUBTRACT:
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
            
        case BlendMode::SCREEN:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
            break;
    }
}

void OpenGLRenderer::ClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void OpenGLRenderer::DrawLine(float x1, float y1, float x2, float y2, Color color) {
    glBindTexture(GL_TEXTURE_2D, 0);
    
    float vertices[] = {
        x1, y1, color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f,
        x2, y2, color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f
    };
    unsigned int indices[] = { 0, 1 };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void OpenGLRenderer::DrawQuad(float x, float y, float w, float h) {
    glBindVertexArray(m_quadVAO);
    
    float vertices[] = {
        // x, y, u, v
        x,     y + h, 0.0f, 1.0f, // top-left
        x + w, y + h, 1.0f, 1.0f, // top-right
        x + w, y,     1.0f, 0.0f, // bottom-right
        x,     y,     0.0f, 0.0f  // bottom-left
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OpenGLRenderer::DrawRect(float x, float y, float w, float h, Color color) {
    glBindTexture(GL_TEXTURE_2D, 0);
    
    float vertices[] = {
        // Position        // Color
        x,     y + h,      color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f,
        x + w, y + h,      color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f,
        x + w, y,          color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f,
        x,     y,          color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void OpenGLRenderer::DrawRectOutline(float x, float y, float w, float h, Color color) {
    DrawLine(x, y, x + w, y, color);
    DrawLine(x + w, y, x + w, y + h, color);
    DrawLine(x + w, y + h, x, y + h, color);
    DrawLine(x, y + h, x, y, color);
}

void OpenGLRenderer::DrawCircle(float x, float y, float radius, Color color, int segments) {
    std::vector<float> vertices;
    vertices.insert(vertices.end(), {
        x, y, color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f
    });
    
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);
        
        vertices.insert(vertices.end(), {
            px, py, color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f
        });
    }
    
    std::vector<uint32_t> indices;
    for (int i = 0; i < segments; i++) {
        indices.insert(indices.end(), {0, (uint32_t)(i + 1), (uint32_t)(i + 2)});
    }
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

}  // namespace Cleave