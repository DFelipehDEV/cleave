#include "rendering/OpenGLRenderer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "thirdparty/stb_image.h"

namespace Cleave {
OpenGLRenderer::~OpenGLRenderer() { Terminate(); }

void OpenGLRenderer::Initialize(Window& window) {
    glfwMakeContextCurrent(window.getGLFWwindow());
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertices[] = {
        // x, y, u, v
        0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        1.0f, 1.0f, 1.0f, 1.0f,  // top-right
        1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
        0.0f, 0.0f, 0.0f, 0.0f   // bottom-left
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

    glBindVertexArray(0);
}

void OpenGLRenderer::Terminate() {}

void OpenGLRenderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto cmd : m_renderCommands) delete cmd;
    m_renderCommands.clear();
}

void OpenGLRenderer::EndFrame() {
}

Matrix4 OpenGLRenderer::GetProjection() const { return m_projection; }
void OpenGLRenderer::SetProjection(Matrix4 projection) {
    m_projection = projection;
}

Rect4f OpenGLRenderer::GetViewPort() const { return m_viewport; }
void OpenGLRenderer::SetViewPort(Rect4f viewport) {
    m_viewport = viewport;
    glViewport(static_cast<GLsizei>(viewport.x), static_cast<GLsizei>(viewport.y), static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
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

void OpenGLRenderer::UseShader(ShaderHandle handle) {
    glUseProgram(m_shaders[handle]);
    m_currentShader = m_shaders[handle];
}

void OpenGLRenderer::SetShaderUniformInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform1i(location, value);
}

void OpenGLRenderer::SetShaderUniformFloat(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform1f(location, value);
}

void OpenGLRenderer::SetShaderUniformVector2f(const std::string& name, float x,
                                float y) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform2f(location, x, y);
}

void OpenGLRenderer::SetShaderUniformVector3f(const std::string& name, float x, float y,
                                float z) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform3f(location, x, y, z);
}

void OpenGLRenderer::SetShaderUniformVector4f(const std::string& name, float x, float y,
                                float z, float w) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform4f(location, x, y, z, w);
}

void OpenGLRenderer::SetShaderUniformMatrix4(const std::string& name,
                               const float* matrix) const {
    GLint location = glGetUniformLocation(m_currentShader, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniformMatrix4fv(location, 1, false, matrix);
}

void OpenGLRenderer::UseTexture(TextureHandle handle) {
    glBindTexture(GL_TEXTURE_2D, m_textures[handle]);
    m_currentTexture = m_textures[handle];
}

Renderer::TextureInfo OpenGLRenderer::CreateTexture(const std::string& path) {
    Renderer::TextureInfo info;
    GLuint glHandle;

    glGenTextures(1, &glHandle);
    glBindTexture(GL_TEXTURE_2D, glHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int channels;
    unsigned char* data = stbi_load(path.c_str(), &info.width, &info.height, &channels, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load texture from file: " << path << std::endl;
        return info;
    }
    TextureHandle handle = NEXT_TEXTURE_HANDLE++;
    m_textures[handle] = glHandle;

    GLenum format;
    switch (channels) {
        case 1:
            format = GL_RED;
            info.format = TextureFormat::R;
            break;
        case 2:
            format = GL_RG;
            info.format = TextureFormat::RG;
            break;
        case 3:
            format = GL_RGB;
            info.format = TextureFormat::RGB;
            break;
        default:
            format = GL_RGBA;
            info.format = TextureFormat::RGBA;
            break;
    }

    info.handle = handle;

    GLenum internalFormat = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, info.width, info.height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return info;
}

ShaderHandle OpenGLRenderer::CreateShader(const std::string& vertex, const std::string& fragment) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vertex.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fragment.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        return -1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ShaderHandle handle = NEXT_SHADER_HANDLE++;
    m_shaders[handle] = shaderProgram;

    return handle;
}

const std::vector<RenderCommand*>& OpenGLRenderer::GetRenderCommands() const { return m_renderCommands; }
void OpenGLRenderer::AddRenderCommand(RenderCommand* command) { m_renderCommands.push_back(command); }

void OpenGLRenderer::RunRenderCommands() {
    std::stable_sort(m_renderCommands.begin(), m_renderCommands.end(),
              [](RenderCommand* a, RenderCommand* b) { return a->depth < b->depth; });

    for (RenderCommand* command : m_renderCommands) {
        command->Run(this);
    }
}

void OpenGLRenderer::ClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void OpenGLRenderer::DrawLine(float x1, float y1, float x2, float y2, Color color) {
    glBindTexture(GL_TEXTURE_2D, 0);

    float vertices[] = {
        x1, y1, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f,
        x2, y2, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
    unsigned int indices[] = {0, 1};

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
    DrawQuad(x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
}

void OpenGLRenderer::DrawQuad(float x, float y, float w, float h,
                              float u0, float v0, float u1, float v1) {
    glBindVertexArray(m_quadVAO);

    float vertices[] = {
        // x, y, u, v
        x, y + h, u0, v1,      // top-left
        x + w, y + h, u1, v1,  // top-right
        x + w, y, u1, v0,      // bottom-right
        x, y, u0, v0           // bottom-left
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
        x, y + h, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f,
        x + w, y + h, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f,
        x + w, y, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f,
        x, y, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};

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
    vertices.insert(vertices.end(), {x, y, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f});

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * (float)M_PI * i / segments;
        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);

        vertices.insert(vertices.end(), {px, py, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f});
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

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

}  // namespace Cleave