#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.hpp"
#include "Services.hpp"
#include "rendering/Renderer.hpp"
#include "resources/Texture.hpp"
#include "resources/ResourceManager.hpp"
#include "entities/Sprite.hpp"

#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    FragColor = texture(tex, TexCoord);
}
)";

int main() {
    Window window(512, 288, "CleaveRT!");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->AddTexture("cat.png");
    
    Services::Provide(resourceManager);

    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    resourceManager->AddShaderFromString("main", vertexShaderSource, fragmentShaderSource);

    Texture* catTexture = resourceManager->textures["cat.png"];

    Sprite* cat = new Sprite({16, 32}, catTexture);
    float x = 0;
    while (!window.shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        resourceManager->shaders["main"]->Use();
        resourceManager->shaders["main"]->SetUniformInt("tex", 0);
        resourceManager->shaders["main"]->SetUniformMatrix4("projection", glm::value_ptr(renderer->m_projection));
        cat->OnRender((Renderer*) renderer);

        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}