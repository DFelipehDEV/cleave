#include "Renderer.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

OpenGLRenderer::~OpenGLRenderer() {
    Terminate();
}

void OpenGLRenderer::Initialize(Window& window) {
    glfwMakeContextCurrent(window.getGLFWwindow());
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::Terminate() {

}

void OpenGLRenderer::ClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void OpenGLRenderer::DrawTexture(Texture& texture, float x, float y) {
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    int w = texture.GetWidth();
    int h = texture.GetHeight();

    // Correct vertex data with proper texture coordinates
    float vertices[] = {
        // Positions    // Texture Coords (flipped to match Y Down)
        x,     y + h,   0.0f, 1.0f,  // Bottom-left (now maps to texture bottom-left)
        x + w, y + h,   1.0f, 1.0f,  // Bottom-right
        x + w, y,       1.0f, 0.0f,  // Top-right
        x,     y,       0.0f, 0.0f   // Top-left
    };    

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}