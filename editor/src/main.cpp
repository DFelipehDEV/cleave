#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.hpp"
#include "Services.hpp"
#include "rendering/Renderer.hpp"
#include "resources/Texture.hpp"
#include "resources/ResourceManager.hpp"
#include "entities/Sprite.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <Scene.hpp>
#include <algorithm>
#include <Editor.hpp>
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

GLuint framebuffer;
GLuint framebufferTexture;

void CreateGameFramebuffer(int width, int height) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main() {
    Window window(512, 288, "CleaveRT!");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->AddTexture("cat.png");
    resourceManager->AddTexture("dog.png");
    
    Services::Provide(resourceManager);

    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    CreateGameFramebuffer(window.GetWidth(), window.GetHeight());

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
        // Clear and render game
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            renderer->ClearColor(100, 149, 237, 255);
            glClear(GL_COLOR_BUFFER_BIT);
            scene->Render((Renderer*)renderer);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(200, window.GetHeight()), ImGuiCond_Once);

            ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);

            editor.GetHierarchy()->OnRender();
            static float entitiesPanelHeight = 200.0f;
            
            // Splitter
            ImGui::InvisibleButton("Splitter", ImVec2(-1, 4.0f));
            if (ImGui::IsItemActive()) {
                entitiesPanelHeight += ImGui::GetIO().MouseDelta.y;
            }

            // Properties panel
            ImGui::BeginChild("PropertiesPanel", ImVec2(0, 0), true);

            ImGui::Text("Properties");

            Entity* selectedEntity = editor.GetHierarchy()->GetSelectedEntity();
            if (selectedEntity) {
                ImGui::Text(selectedEntity->GetName().c_str());
                ImGui::Text("Name:");
                char nameBuffer[32];
                strncpy(nameBuffer, selectedEntity->GetName().c_str(), sizeof(nameBuffer));
                nameBuffer[sizeof(nameBuffer) - 1] = '\0';

                if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
                    selectedEntity->SetName(nameBuffer);
                }

                ImGui::Text("Transform:");
                Transform entityTransform = selectedEntity->GetTransform();

                ImGui::Text("Position:");
                Vec2f position = entityTransform.GetPosition();
                if (ImGui::InputFloat2("##Position", &position.x)) {
                    selectedEntity->GetTransform().SetPosition(position);
                }

                ImGui::Text("Scale:");
                Vec2f scale = entityTransform.GetScale();
                if (ImGui::InputFloat2("##Scale", &scale.x)) {
                    selectedEntity->GetTransform().SetScale(scale);
                }

                ImGui::Text("Rotation:");
                float rotation = entityTransform.GetRotationDegrees();
                if (ImGui::InputFloat("##Rotation", &rotation)) {
                    selectedEntity->GetTransform().SetRotationDegrees(rotation);
                }
            }

            ImGui::EndChild();
            ImGui::End();
            ImGui::Begin("Game View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            static ImVec2 lastSize = { 512, 288 };
            ImVec2 contentSize = ImGui::GetContentRegionAvail();

            if (contentSize.x != lastSize.x || contentSize.y != lastSize.y) {
                lastSize = contentSize;
            
                // Recreate the framebuffer with the new size
                glDeleteTextures(1, &framebufferTexture);
                glDeleteFramebuffers(1, &framebuffer);
                CreateGameFramebuffer(static_cast<uint32_t>(contentSize.x), static_cast<uint32_t>(contentSize.y));
                
                renderer->SetViewPort(Rect4f(0.0f, 0.0f, contentSize.x, contentSize.y));
                renderer->SetProjection(glm::ortho(
                    0.0f, contentSize.x,
                    contentSize.y, 0.0f,
                    -1.0f, 1.0f
                ));
            }
            
            ImGui::Image((ImTextureID)(intptr_t)framebufferTexture, contentSize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
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