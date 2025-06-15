#include "Hierarchy.hpp"

#include <scene/EntityRegistry.hpp>

#include "entities/Sprite.hpp"
#include "imgui.h"

namespace Cleave {
namespace Editor {
void ShowEntityHierarchy(Entity* entity, Entity*& selectedEntity) {
    if (!entity) return;
    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
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
    if (GetScene() && GetScene()->GetRoot())
        ShowEntityHierarchy(GetScene()->GetRoot(), m_selectedEntity);
    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu",
                                       ImGuiPopupFlags_MouseButtonRight)) {
        static int entityCount = 0;
        if (ImGui::Selectable("New Entity")) {
            Entity* newEntity = new Entity();
            newEntity->SetName("Entity" + std::to_string(entityCount++));
            m_selectedEntity->AddChild(newEntity);
        }

        for (auto registry : Registry::GetAllTypes()) {
            if (ImGui::Selectable(("New " + registry.first).c_str())) {
                Entity* entity = Registry::CreateEntity(registry.first);
                entity->SetName(registry.first + std::to_string(entityCount++));
                m_selectedEntity->AddChild(entity);
            }
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

Scene* Hierarchy::GetScene() { return m_gameView->GetScene(); }
void Hierarchy::SetScene(std::shared_ptr<Scene> scene) {
    if (m_gameView && scene) {
        m_gameView->SetScene(scene);
        m_selectedEntity = scene->GetRoot();
    }
}

Entity* Hierarchy::GetSelectedEntity() { return m_selectedEntity; }
}  // namespace Editor
}  // namespace Cleave