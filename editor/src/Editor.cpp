#include "Editor.hpp"
#include "imgui.h"
#include <iostream>

Editor::Editor() {
	m_scene = std::make_shared<Scene>();
	Entity* root = new Entity(Transform({ 0, 0 }));
	root->SetName("Root");
	m_scene->SetRoot(root);

	m_hierarchy = std::make_shared<Hierarchy>(GetScene()->GetRoot());
	m_properties = std::make_shared<Properties>();
	m_gameView = std::make_shared<GameView>(GetScene());
}

void Editor::OnRender(Renderer *renderer) {
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

Scene* Editor::GetScene() { return m_scene.get(); }

Hierarchy* Editor::GetHierarchy() { return m_hierarchy.get(); }

Properties* Editor::GetProperties() { return m_properties.get(); }

GameView* Editor::GetGameView() { return m_gameView.get(); }