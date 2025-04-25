#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Window.hpp"
#include "Services.hpp"
#include "rendering/Renderer.hpp"
#include "resources/Texture.hpp"
#include "resources/ResourceManager.hpp"
#include "entities/Sprite.hpp"

#include "Editor.hpp"
#include "GameView.hpp"
#include "Scene.hpp"
#include "Properties.hpp"

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

const char* spriteVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* spriteFragmentShaderSource = R"(
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
    resourceManager->AddTexture("dog.png");
    
    Services::Provide(resourceManager);

    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    resourceManager->AddShaderFromString("main", vertexShaderSource, fragmentShaderSource);
    resourceManager->AddShaderFromString("sprite", spriteVertexShaderSource, spriteFragmentShaderSource);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    Editor editor = Editor();
    Scene* scene = editor.GetScene();

    Sprite* cat = new Sprite(Transform({ 16, 32 }), resourceManager->textures["cat.png"]);
    cat->SetName("Carlos Gato");

    Sprite* dog = new Sprite(Transform({ 128, 128 }), resourceManager->textures["dog.png"]);
    dog->SetName("Roberto Cao");
    cat->AddChild(dog);

    scene->GetRoot()->AddChild(cat);
    
    while (!window.shouldClose()) {
        window.pollEvents();
        resourceManager->shaders["main"]->Use();
        resourceManager->shaders["main"]->SetUniformInt("tex", 0);
        resourceManager->shaders["main"]->SetUniformMatrix4("projection", glm::value_ptr(renderer->GetProjection()));
        // Clear
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            editor.OnRender((Renderer*) renderer);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}