#include "Hierarchy.hpp"
#include "imgui.h"
#include "entities/Sprite.hpp"

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
    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
        static int entityCount = 0;
        if (ImGui::Selectable("New Entity")) {
            Entity* newEntity = new Entity();
            newEntity->SetName("Entity" + std::to_string(entityCount++));
            m_selectedEntity->AddChild(newEntity);
        }
        if (ImGui::Selectable("New Sprite")) {
            Sprite* newSprite = new Sprite();
            newSprite->SetName("Sprite" + std::to_string(entityCount++));
            m_selectedEntity->AddChild(newSprite);
        }

        ImGui::EndPopup();
    }
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