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

    Scene scene;
    Entity* root = new Entity(Transform({ 0, 0 }));
    scene.SetRoot(root);

    Sprite* cat = new Sprite(Transform({ 16, 32 }), resourceManager->textures["cat.png"]);
    cat->SetName("Carlos Gato");

    Sprite* dog = new Sprite(Transform({ 128, 128 }), resourceManager->textures["dog.png"]);
    dog->SetName("Roberto Cao");
    root->AddChild(dog);

    root->AddChild(cat);

    Entity* selectedEntity = nullptr;
    
    while (!window.shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        window.pollEvents();
        resourceManager->shaders["main"]->Use();
        resourceManager->shaders["main"]->SetUniformInt("tex", 0);
        resourceManager->shaders["main"]->SetUniformMatrix4("projection", glm::value_ptr(renderer->m_projection));
        scene.Render((Renderer*)renderer);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(200, window.GetHeight()), ImGuiCond_Once);
            ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoCollapse);

            static float entitiesPanelHeight = 200.0f;

            // Entities list
            ImGui::BeginChild("EntitiesList", ImVec2(0, entitiesPanelHeight), true);
            for (Entity* child : root->GetChildren()) {
                if (ImGui::Selectable(child->GetName().c_str(), selectedEntity == child)) {
                    selectedEntity = child;
                }
            }
            ImGui::EndChild();

            // Splitter
            ImGui::InvisibleButton("Splitter", ImVec2(-1, 4.0f));
            if (ImGui::IsItemActive()) {
                entitiesPanelHeight += ImGui::GetIO().MouseDelta.y;
            }

            // Properties panel
            ImGui::BeginChild("PropertiesPanel", ImVec2(0, 0), true);

            ImGui::Text("Properties");

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
                float rotation = entityTransform.GetRotation();
                if (ImGui::InputFloat("##Rotation", &rotation)) {
                    selectedEntity->GetTransform().SetRotationDegrees(rotation);
                }
            }

            ImGui::EndChild();
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