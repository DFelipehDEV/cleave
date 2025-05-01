#include "Properties.hpp"
#include "imgui.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

void Properties::OnRender() {
    if (m_entity) {
        EntityID id = m_entity->GetID();
        EditorEntityProperties& entityProperties = m_editorProperties[id];

        ImGui::Text("Name:");
        char nameBuffer[32];
        strncpy_s(nameBuffer, m_entity->GetName().c_str(), sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer) - 1] = '\0';

        if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
            m_entity->SetName(nameBuffer);
        }

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

        ImGui::Text("Header:");
        char headerBuffer[256];
        strncpy_s(headerBuffer, entityProperties.headerPath.c_str(), sizeof(headerBuffer));
        headerBuffer[sizeof(headerBuffer) - 1] = '\0';
    
        if (ImGui::InputText("##Header", headerBuffer, sizeof(headerBuffer))) {
            entityProperties.headerPath = headerBuffer;
            entityProperties.properties = ParseEntityHeader(headerBuffer);
        }
    
        if (!entityProperties.properties.empty()) {
            for (auto& [name, type] : entityProperties.properties) {
                std::string& valueStr = entityProperties.values[name];
        
                if (type == "Vec2f") {
                    Vec2f vec = Vec2f::FromString(valueStr);
                    if (ImGui::InputFloat2(name.c_str(), &vec.x)) {
                        std::ostringstream ss;
                        ss << vec.x << "," << vec.y;
                        valueStr = ss.str();
                    }
                } else if (type == "float") {
                    float val = valueStr.empty() ? 0.0f : std::stof(valueStr);
                    if (ImGui::InputFloat(name.c_str(), &val)) {
                        valueStr = std::to_string(val);
                    }
                } else if (type == "string") {
                    char buffer[256];
                    strncpy_s(buffer, valueStr.c_str(), sizeof(buffer));
                    buffer[sizeof(buffer) - 1] = '\0';
                    if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
                        valueStr = buffer;
                    }
                } else {
                    ImGui::Text("Unknown type: %s", type.c_str());
                }
            }
        }        
    }
}

Entity *Properties::GetEntity() const { return m_entity; }
void Properties::SetEntity(Entity *entity) { m_entity = entity; }

std::unordered_map<std::string, std::string> Properties::ParseEntityHeader(const std::string& headerPath) const {
    std::unordered_map<std::string, std::string> properties;
    std::ifstream file(headerPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << headerPath << std::endl;
        return properties;
    }
    
    std::regex pattern(R"(^\s*\/\/\s*@prop\s+(\w+)\s*:\s*(\w+))");
    std::string line;

    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, pattern)) {
            properties[match[1]] = match[2];
        }
    }

    return properties;
}