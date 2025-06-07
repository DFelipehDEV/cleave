#include "Editor.hpp"
#include "imgui.h"

#include <iostream>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Services.hpp"
#include "scene/JsonSceneSerializer.hpp"

Editor::Editor(Window* window) : m_window(window) {
	m_gameView = std::make_shared<GameView>(std::make_shared<Scene>(std::make_unique<Entity>(Transform({ 0, 0 }), "root")));
	m_hierarchy = std::make_shared<Hierarchy>(m_gameView);
	m_properties = std::make_shared<Properties>();
}

void Editor::Run(Renderer *renderer) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsClassic();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window->getGLFWwindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

	while (!m_window->shouldClose()) {
        m_window->pollEvents();
		auto resourceManager = Services::GetResourceManager();
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
			ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);
				m_hierarchy->OnRender();
				m_properties->SetEntity(m_hierarchy->GetSelectedEntity());

				// Properties panel
				ImGui::BeginChild("PropertiesPanel", ImVec2(0, 0), true);
					m_properties->OnRender();
				ImGui::EndChild();
			ImGui::End();

			ImGui::Begin("Game View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				m_gameView->OnUpdate();
				m_gameView->OnRender((Renderer*)renderer);
			ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->swapBuffers();
    }

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Hierarchy* Editor::GetHierarchy() { return m_hierarchy.get(); }

Properties* Editor::GetProperties() { return m_properties.get(); }

GameView* Editor::GetGameView() { return m_gameView.get(); }