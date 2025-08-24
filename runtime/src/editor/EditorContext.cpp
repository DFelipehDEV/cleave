#include "EditorContext.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>
#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

#include "Services.hpp"
#include "imgui.h"
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "scene/JsonSceneSerializer.hpp"

namespace Cleave {
namespace Editor {
EditorContext::EditorContext(Window* window) : m_window(window) {
    m_properties = std::make_shared<Properties>();
    m_gameView = std::make_shared<GameView>(std::make_shared<Scene>(
        std::make_unique<Entity>(Transform({0, 0}), "root")), m_properties);
    m_hierarchy = std::make_shared<Hierarchy>(m_gameView);
    m_fileExplorer =
        std::make_shared<FileExplorer>(std::filesystem::current_path(), m_gameView.get());
    m_menuBar = std::make_shared<MainMenuBar>(this, m_fileExplorer.get());
}

void EditorContext::Run(Renderer* renderer) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::StyleColorsClassic();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        m_window->getGLFWwindow(),
        true);  // Second param install_callback=true will install GLFW
                // callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    while (!m_window->shouldClose()) {
        m_window->pollEvents();
        auto resourceManager = GET_RESMGR();
        auto shader = resourceManager->Get<Shader>("res/shaders/main.vert");
        shader->Use();
        shader->SetUniformInt("tex", 0);
        shader->SetUniformMatrix4("projection",
                                  glm::value_ptr(renderer->GetProjection()));
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
            m_menuBar->OnRender();
            if (IsHierarchyVisible()) {
                ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);
                m_hierarchy->OnRender();
                if (m_hierarchy->GetSelectedEntity()) {
                    m_properties->SetEntityId(m_hierarchy->GetSelectedEntity()->GetId());
                }

                // Properties panel
                ImGui::BeginChild("PropertiesPanel", ImVec2(0, 0), true);
                m_properties->OnRender(m_gameView->GetScene());
                ImGui::EndChild();
                ImGui::End();
            }

            if (IsFileExplorerVisible()) {
                ImGui::Begin("File Explorer", nullptr,
                             ImGuiWindowFlags_NoCollapse);
                m_fileExplorer->OnRender();
                ImGui::End();
            }

            if (IsGameViewVisible()) {
                ImGui::Begin("Game View", nullptr,
                             ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoScrollWithMouse);
                m_gameView->OnUpdate();
                m_gameView->OnRender((Renderer*)renderer);
                ImGui::End();
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Hierarchy* EditorContext::GetHierarchy() { return m_hierarchy.get(); }

FileExplorer* EditorContext::GetFileExplorer() { return m_fileExplorer.get(); }

Properties* EditorContext::GetProperties() { return m_properties.get(); }

GameView* EditorContext::GetGameView() { return m_gameView.get(); }

bool EditorContext::IsHierarchyVisible() const { return m_hierarchyVisible; }
void EditorContext::SetHierarchyVisible(bool visible) { m_hierarchyVisible = visible; }

bool EditorContext::IsFileExplorerVisible() const { return m_fileExplorerVisible; }
void EditorContext::SetFileExplorerVisible(bool visible) {
    m_fileExplorerVisible = visible;
}

bool EditorContext::IsGameViewVisible() const { return m_gameViewVisible; }
void EditorContext::SetGameViewVisible(bool visible) { m_gameViewVisible = visible; }

}  // namespace Editor
}  // namespace Cleave