#include "Properties.hpp"
#include "imgui.h"

void Properties::OnRender() {
    ImGui::Text("Properties");

    if (m_entity) {
        ImGui::Text(m_entity->GetName().c_str());
        ImGui::Text("Name:");
        char nameBuffer[32];
        strncpy_s(nameBuffer, m_entity->GetName().c_str(), sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer) - 1] = '\0';

        if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
            m_entity->SetName(nameBuffer);
        }

        ImGui::Text("Transform:");
        Transform entityTransform = m_entity->GetTransform();

        ImGui::Text("Position:");
        Vec2f position = entityTransform.GetPosition();
        if (ImGui::InputFloat2("##Position", &position.x)) {
            m_entity->GetTransform().SetPosition(position);
        }

        ImGui::Text("Scale:");
        Vec2f scale = entityTransform.GetScale();
        if (ImGui::InputFloat2("##Scale", &scale.x)) {
            m_entity->GetTransform().SetScale(scale);
        }

        ImGui::Text("Rotation:");
        float rotation = entityTransform.GetRotationDegrees();
        if (ImGui::InputFloat("##Rotation", &rotation)) {
            m_entity->GetTransform().SetRotationDegrees(rotation);
        }
    }
}

Entity *Properties::GetEntity() const { return m_entity; }
void Properties::SetEntity(Entity *entity) { m_entity = entity; }
