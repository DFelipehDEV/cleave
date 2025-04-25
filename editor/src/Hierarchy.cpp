#include "Hierarchy.hpp"
#include "imgui.h"

void ShowEntityHierarchy(Entity* entity, Entity*& selectedEntity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (entity == selectedEntity) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    std::string label = " ";
    label += entity->GetName().c_str();
    bool opened = ImGui::TreeNodeEx(label.c_str(), flags);
    if (ImGui::IsItemClicked()) {
        selectedEntity = entity;
    }

    if (opened) {
        for (Entity* child : entity->GetChildren()) {
            ShowEntityHierarchy(child, selectedEntity);
        }
        ImGui::TreePop();
    }
}

void Hierarchy::OnRender() {
    ImGui::BeginChild("Hierarchy", ImVec2(0, 200), true);
    ShowEntityHierarchy(m_root, m_selectedEntity);
    ImGui::EndChild();

    static float entitiesPanelHeight = 200.0f;
            
    // Splitter
    ImGui::InvisibleButton("Splitter", ImVec2(-1, 4.0f));
    if (ImGui::IsItemActive()) {
        entitiesPanelHeight += ImGui::GetIO().MouseDelta.y;
    }
}

Entity* Hierarchy::GetRoot() { return m_root; }
void Hierarchy::SetRoot(Entity* root) { m_root = root; }

Entity* Hierarchy::GetSelectedEntity() { return m_selectedEntity; }