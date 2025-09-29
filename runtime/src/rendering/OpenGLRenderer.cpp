#include "rendering/OpenGLRenderer.hpp"

#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "thirdparty/stb_image.h"
#include "Log.hpp"

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

    if (FT_Init_FreeType(&m_ftLibrary)) {
        LOG_ERROR("Couldn't init FreeType Library");
    }
}

void OpenGLRenderer::Terminate() {
    FT_Done_FreeType(m_ftLibrary);
}

void OpenGLRenderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_drawCalls = 0; 
    m_renderCommands.clear();
}

void OpenGLRenderer::EndFrame() {
}

uint32_t OpenGLRenderer::GetDrawCalls() const { return m_drawCalls; }

int OpenGLRenderer::GetDepth() const { return m_depth; }
void OpenGLRenderer::SetDepth(int depth) { m_depth = depth; }

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

void OpenGLRenderer::SetShader(ShaderHandle handle) {
    m_currentShader = handle;
}

void OpenGLRenderer::UseShader(ShaderHandle handle) {
    glUseProgram(m_shaders[handle]);
    SetShader(handle);
}

void OpenGLRenderer::SetShaderUniformInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader << " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniform1i(location, value);
}

void OpenGLRenderer::SetShaderUniformFloat(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader << " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniform1f(location, value);
}

void OpenGLRenderer::SetShaderUniformVector2f(const std::string& name, float x,
                                float y) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader << " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniform2f(location, x, y);
}

void OpenGLRenderer::SetShaderUniformVector3f(const std::string& name, float x, float y,
                                float z) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader << " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniform3f(location, x, y, z);
}

void OpenGLRenderer::SetShaderUniformVector4f(const std::string& name, float x, float y,
                                float z, float w) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader <<  " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniform4f(location, x, y, z, w);
}

void OpenGLRenderer::SetShaderUniformMatrix4(const std::string& name,
                               Matrix4 matrix) const {
    GLint location = glGetUniformLocation(m_shaders.at(m_currentShader), name.c_str());
    if (location == -1) {
        LOG_WARN("Shader: " << m_currentShader <<  " ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")");
        return;
    }
    glUniformMatrix4fv(location, 1, false, (float*)matrix.m);
}

void OpenGLRenderer::SetTexture(TextureHandle handle) {
    m_currentTexture = handle;
}

void OpenGLRenderer::UseTexture(TextureHandle handle) {
    glBindTexture(GL_TEXTURE_2D, m_textures[handle]);
    SetTexture(handle);
}

Renderer::TextureInfo OpenGLRenderer::CreateFallbackTexture() {
    Renderer::TextureInfo info;
    info.width = 2;
    info.height = 2;
    info.format = TextureFormat::RGBA;

    unsigned char pixels[] = {
        255, 0, 255, 255,   0, 0, 0, 255,
        0, 0, 0, 255,       255, 0, 255, 255
    };

    GLuint glHandle;
    glGenTextures(1, &glHandle);
    glBindTexture(GL_TEXTURE_2D, glHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    TextureHandle handle = NEXT_TEXTURE_HANDLE++;
    m_textures[handle] = glHandle;
    info.handle = handle;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return info;
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
        LOG_ERROR("Failed to load texture from file: " << path);
        return CreateFallbackTexture();
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
        LOG_ERROR("Vertex shader compilation failed: " << infoLog);
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
        LOG_ERROR("Fragment shader compilation failed: " << infoLog);
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
        LOG_ERROR("Shader program linking failed: " << infoLog);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ShaderHandle handle = NEXT_SHADER_HANDLE++;
    m_shaders[handle] = shaderProgram;

    return handle;
}

FontHandle OpenGLRenderer::CreateFont(const std::string& fontPath, int fontSize) {
    FT_Face face;
    if (FT_New_Face(m_ftLibrary, fontPath.c_str(), 0, &face)) {
        LOG_ERROR("Failed to load font: " << fontPath);
        return 0;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    
    FontHandle handle = NEXT_FONT_HANDLE++;
    std::unordered_map<char, Glyph> glyphs;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load ASCII characters
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            LOG_WARN("Failed to load Glyph for character: " << (int)c);
            continue;
        }

        Glyph glyph;
        glyph.size = {static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)};
        glyph.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        glyph.advance = face->glyph->advance.x;
        
        // Create texture only if glyph has bitmap data
        if (glyph.size.x > 0 && glyph.size.y > 0) {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                        glyph.size.x, glyph.size.y, 0,
                        GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glyph.texture = NEXT_TEXTURE_HANDLE++;
            m_textures[glyph.texture] = texture;
        }
        
        glyphs[c] = glyph;
    }
    
    m_fonts[handle] = std::move(glyphs);
    
    FT_Done_Face(face);
    return handle;
}

const std::vector<std::unique_ptr<RenderCommand>>& OpenGLRenderer::GetRenderCommands() const { return m_renderCommands; }
void OpenGLRenderer::AddRenderCommand(std::unique_ptr<RenderCommand> command) { m_renderCommands.push_back(std::move(command)); }

void OpenGLRenderer::RunRenderCommands() {
    std::stable_sort(m_renderCommands.begin(), m_renderCommands.end(),
        [](const std::unique_ptr<RenderCommand>& a, const std::unique_ptr<RenderCommand>& b) {
            return a->depth < b->depth;
        });

    for (auto& command : m_renderCommands) {
        if (!command) continue;
        RenderCommand* rawCmd = command.get();
        Transform transform = Transform();
        switch (rawCmd->type) {
            case RenderCommand::Type::Quad: {
                RenderQuadCommand* quadCmd = dynamic_cast<RenderQuadCommand*>(rawCmd);
                if (!quadCmd) break;
                transform.SetPosition({quadCmd->x, quadCmd->y});
                transform.SetRotation(quadCmd->rotation);
                transform.SetScale({quadCmd->scaleX, quadCmd->scaleY});
                if (quadCmd->shader != -1) {
                    UseShader(quadCmd->shader);
                    SetShaderUniformInt("tex", 0);
                    SetShaderUniformMatrix4("projection", GetProjection());
                    SetShaderUniformMatrix4("model", transform.GetMatrix());
                    SetShaderUniformVector4f("color", quadCmd->color.r / 255.0f, quadCmd->color.g / 255.0f, quadCmd->color.b / 255.0f, quadCmd->color.a / 255.0f);
                }

                if (quadCmd->texture != -1) {
                    UseTexture(quadCmd->texture);
                }
                glBindVertexArray(m_quadVAO);

                float vertices[] = {
                    // x, y, u, v
                    quadCmd->x, quadCmd->y + quadCmd->h, quadCmd->u0, quadCmd->v1,     // top-left
                    quadCmd->x + quadCmd->w, quadCmd->y + quadCmd->h, quadCmd->u1, quadCmd->v1,   // top-right
                    quadCmd->x + quadCmd->w, quadCmd->y, quadCmd->u1, quadCmd->v0,     // bottom-right
                    quadCmd->x, quadCmd->y, quadCmd->u0, quadCmd->v0             // bottom-left
                };

                glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                m_drawCalls++;
                glBindVertexArray(0);
                break;
            }

            case RenderCommand::Type::Line: {
                RenderLineCommand* lineCmd = dynamic_cast<RenderLineCommand*>(rawCmd);
                if (!lineCmd) break;

                if (rawCmd->shader != -1) { 
                    UseShader(rawCmd->shader);
                    SetShaderUniformMatrix4("projection", GetProjection());
                    //SetShaderUniformMatrix4("transform", transform);
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                float vertices[] = {
                    lineCmd->x1, lineCmd->y1, lineCmd->color.r / 255.0f, lineCmd->color.g / 255.0f, lineCmd->color.b / 255.0f, lineCmd->color.a / 255.0f,
                    lineCmd->x2, lineCmd->y2, lineCmd->color.r / 255.0f, lineCmd->color.g / 255.0f, lineCmd->color.b / 255.0f, lineCmd->color.a / 255.0f};
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
                m_drawCalls++;

                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                break;
            }

            case RenderCommand::Type::Rect: {
                RenderRectCommand* rectCmd = dynamic_cast<RenderRectCommand*>(rawCmd);
                if (!rectCmd) break;
                if (rawCmd->shader != -1) { 
                    UseShader(rawCmd->shader);
                    SetShaderUniformMatrix4("projection", GetProjection());
                    //SetShaderUniformMatrix4("transform", transform);
                }
                glBindTexture(GL_TEXTURE_2D, 0);

                float vertices[] = {
                    // Position        // Color
                    rectCmd->x, rectCmd->y + rectCmd->h, rectCmd->color.r / 255.0f, rectCmd->color.g / 255.0f, rectCmd->color.b / 255.0f, rectCmd->color.a / 255.0f,
                    rectCmd->x + rectCmd->w, rectCmd->y + rectCmd->h, rectCmd->color.r / 255.0f, rectCmd->color.g / 255.0f, rectCmd->color.b / 255.0f, rectCmd->color.a / 255.0f,
                    rectCmd->x + rectCmd->w, rectCmd->y, rectCmd->color.r / 255.0f, rectCmd->color.g / 255.0f, rectCmd->color.b / 255.0f, rectCmd->color.a / 255.0f,
                    rectCmd->x, rectCmd->y, rectCmd->color.r / 255.0f, rectCmd->color.g / 255.0f, rectCmd->color.b / 255.0f, rectCmd->color.a / 255.0f};

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
                m_drawCalls++;

                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                break;
            }

            case RenderCommand::Type::Circle: {
                RenderCircleCommand* circleCmd = dynamic_cast<RenderCircleCommand*>(rawCmd);
                if (!circleCmd) break;
                if (rawCmd->shader != -1) { 
                    UseShader(rawCmd->shader);
                    SetShaderUniformMatrix4("projection", GetProjection());
                    //SetShaderUniformMatrix4("transform", transform);
                }
                std::vector<float> vertices;
                vertices.insert(vertices.end(), {circleCmd->x, circleCmd->y, circleCmd->color.r / 255.0f, circleCmd->color.g / 255.0f, circleCmd->color.b / 255.0f, circleCmd->color.a / 255.0f});

                for (int i = 0; i <= circleCmd->segments; i++) {
                    float angle = 2.0f * (float)M_PI * i / circleCmd->segments;
                    float px = circleCmd->x + circleCmd->radius * cos(angle);
                    float py = circleCmd->y + circleCmd->radius * sin(angle);

                    vertices.insert(vertices.end(), {px, py, circleCmd->color.r / 255.0f, circleCmd->color.g / 255.0f, circleCmd->color.b / 255.0f, circleCmd->color.a / 255.0f});
                }

                std::vector<uint32_t> indices;
                for (int i = 0; i < circleCmd->segments; i++) {
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
                m_drawCalls++;

                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                break;
            }
        }
    }
}

void OpenGLRenderer::ClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void OpenGLRenderer::DrawQuad(float x, float y, float w, float h, float u0, float v0, float u1, float v1, Color color) {
    AddRenderCommand(
        std::make_unique<RenderQuadCommand>(
            x, y, w, h, 1.0f, 1.0f, 
            0.0f, m_currentTexture, m_currentShader, 
            m_depth, u0, v0, u1, v1, color
        )
    );
}

void OpenGLRenderer::DrawQuad(float x, float y, float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1, float v1, Color color) {
    AddRenderCommand(
        std::make_unique<RenderQuadCommand>(
            x, y, w, h, scaleX, scaleY, 
            rotation, m_currentTexture, m_currentShader, 
            m_depth, u0, v0, u1, v1, color
        )
    );
}

void OpenGLRenderer::DrawLine(float x1, float y1, float x2, float y2, Color color) {
    AddRenderCommand(std::make_unique<RenderLineCommand>(x1, y1, x2, y2, color, m_currentShader, m_depth));
}

void OpenGLRenderer::DrawRect(float x, float y, float w, float h, Color color) {
    AddRenderCommand(std::make_unique<RenderRectCommand>(x, y, w, h, color, m_currentShader, m_depth));
}

void OpenGLRenderer::DrawRectOutline(float x, float y, float w, float h, Color color) {
    DrawLine(x, y, x + w, y, color);
    DrawLine(x + w, y, x + w, y + h, color);
    DrawLine(x + w, y + h, x, y + h, color);
    DrawLine(x, y + h, x, y, color);
}

void OpenGLRenderer::DrawCircle(float x, float y, float radius, Color color, int segments) {
    AddRenderCommand(std::make_unique<RenderCircleCommand>(x, y, radius, color, segments, m_currentShader, m_depth));
}

const Glyph* OpenGLRenderer::GetGlyph(FontHandle fontHandle, char c) {
    auto fontIt = m_fonts.find(fontHandle);
    if (fontIt == m_fonts.end())
        return nullptr;

    auto& glyphs = fontIt->second;
    auto glyphIt = glyphs.find(c);
    if (glyphIt == glyphs.end())
        return nullptr;

    return &glyphIt->second;
}

void OpenGLRenderer::DrawText(const std::string& text, FontHandle fontHandle, 
                              float x, float y, float scale, Color color) {
    const Glyph* glyph;
    float cursorX = x;
    float cursorY = y;

    for (char c : text) {
        glyph = GetGlyph(fontHandle, c);
        if (!glyph || glyph->texture == -1) {
            cursorX += 8 * scale;
            continue;
        }

        float xpos = cursorX + glyph->bearing.x * scale;
        float ypos = cursorY - (glyph->size.y - glyph->bearing.y) * scale;
        float w = glyph->size.x * scale;
        float h = glyph->size.y * scale;

        SetTexture(glyph->texture);
        DrawQuad(xpos, ypos, w, h, 0.0f, 0.0f, 1.0f, 1.0f, color); 

        cursorX += (glyph->advance >> 6) * scale;
    }
}


}  // namespace Cleave