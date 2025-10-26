#include "Hierarchy.hpp"

#include <scene/EntityRegistry.hpp>

#include "entities/Sprite.hpp"
#include "imgui.h"
#include "services/ResourceManager.hpp"

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
    label += entity->GetTypeName();
    label += " (" + entity->GetId() + ")";
    bool opened = ImGui::TreeNodeEx(label.c_str(), flags);
    if (ImGui::IsItemClicked()) {
        selectedEntity = entity;
    }

    if (opened) {
        for (auto& child : entity->GetChildren()) {
            ShowEntityHierarchy(child.get(), selectedEntity);
        }
        ImGui::TreePop();
    }
}

void Hierarchy::OnRender(Scene* scene) {
    ImGui::BeginChild("Hierarchy", ImVec2(0, 200), true);
    auto root = scene->GetRoot();
    if (root)
        ShowEntityHierarchy(root, m_selectedEntity);
    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu",
                                       ImGuiPopupFlags_MouseButtonRight)) {
        static int entityCount = 0;

        if (ImGui::BeginMenu("Create")) {
            for (auto registry : Registry::GetAllTypes()) {
                if (ImGui::Selectable(("New " + registry.first).c_str())) {
                    std::unique_ptr<Entity> entity =
                        Registry::CreateEntity(registry.first);
                    m_selectedEntity->AddChild(std::move(entity));
                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add Subscene")) {
            for (auto& res : GET_RESMGR()->GetAll<Scene>()) {
                const std::string& path = res->GetPath();
                if (ImGui::Selectable(path.c_str())) {
                    scene->AddSubScene(res);
                }
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::Selectable("Delete Entity")) {
            if (m_selectedEntity && m_selectedEntity->GetParent()) {
                Entity* parent = m_selectedEntity->GetParent();
                m_selectedEntity->GetParent()->RemoveChild(m_selectedEntity);
                m_selectedEntity = parent;
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

Entity* Hierarchy::GetSelectedEntity() { return m_selectedEntity; }
}  // namespace Editor
}  // namespace Cleave