#include "EditorContext.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>

#include "services/Services.hpp"
#include "imgui.h"
#include "services/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "scene/JsonSceneSerializer.hpp"

namespace Cleave {
namespace Editor {
EditorContext::EditorContext(Window* window) : m_window(window) {
    m_properties = std::make_shared<Properties>();
    m_gameViews.push_back(std::make_shared<GameView>(std::make_shared<Scene>(
        std::make_unique<Entity>(Transform({0, 0}), "root")), m_properties));
    m_currentGameView = 0;
    m_hierarchy = std::make_shared<Hierarchy>(GetCurrentGameView()->GetScene()->GetRoot());
    m_fileExplorer =
        std::make_shared<FileExplorer>(std::filesystem::current_path(), this);
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

    auto lastPrintTime = std::chrono::high_resolution_clock::now();
    while (!m_window->shouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        m_window->pollEvents();
        auto resourceManager = GET_RESMGR();
        auto shader = resourceManager->Get<Shader>("res/shaders/main.vert");
        renderer->UseShader(shader->GetHandle());
        renderer->SetShaderUniformInt("tex", 0);
        renderer->SetShaderUniformMatrix4("projection", renderer->GetProjection());
        renderer->BeginFrame();
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
                m_hierarchy->OnRender(GetCurrentGameView()->GetScene());
                if (m_hierarchy->GetSelectedEntity()) {
                    m_properties->SetEntityId(m_hierarchy->GetSelectedEntity()->GetId());
                }

                // Properties panel
                ImGui::BeginChild("PropertiesPanel", ImVec2(0, 0), true);
                m_properties->OnRender(GetCurrentGameView()->GetScene());
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
                ImGui::BeginChild("GameViewsPanel", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                if (ImGui::BeginTabBar("GameViewTabs")) {
                    for (const std::shared_ptr<GameView>& gameView : m_gameViews) {
                        std::string tabName = gameView->GetScene()->GetPath();
                        if (ImGui::BeginTabItem(tabName.c_str())) {
                            m_currentGameView = static_cast<int>(&gameView - &m_gameViews[0]);
                            ImGui::EndTabItem();
                        }
                    }
                    ImGui::EndTabBar();
                }

                m_gameViews[m_currentGameView]->OnUpdate();
                m_gameViews[m_currentGameView]->OnRender((Renderer*)renderer);
                ImGui::EndChild();
            }
        }
        renderer->EndFrame();
        auto end = std::chrono::high_resolution_clock::now();
        float frameTimeMs = std::chrono::duration<float, std::milli>(end - now).count();
        float elapsed = std::chrono::duration<float>(end - lastPrintTime).count();
        if (elapsed >= 1.0f) {
            LOG_INFO("Frame Time: " << frameTimeMs 
                        << " FPS:" << (frameTimeMs > 0.0f ? 1000.0f / frameTimeMs : 0.0f) 
                        << " DrawCalls:" << renderer->GetDrawCalls()
                        << " TextureSwaps:" << renderer->GetTextureSwaps());
            lastPrintTime = end;
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

std::shared_ptr<Properties>& EditorContext::GetProperties() { return m_properties; }

std::vector<std::shared_ptr<GameView>>& EditorContext::GetGameViews() { return m_gameViews; }
void EditorContext::AddGameView(std::shared_ptr<GameView> gameView) { m_gameViews.push_back(gameView); }

const std::shared_ptr<GameView>& EditorContext::GetCurrentGameView() { return m_gameViews[m_currentGameView]; }
void EditorContext::SetCurrentGameView(int index) { m_currentGameView = index; }

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