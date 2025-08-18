#include "Properties.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "imgui.h"

namespace Cleave {
namespace Editor {
void Properties::OnRender() {
    if (!m_entity) return;

    EntityID id = m_entity->GetId();
    auto entityProperties = m_entity->GetProperties();

    if (!entityProperties.empty()) {
        bool changed = false;
        std::unordered_map<std::string, Entity::Property> newProperties;
        for (auto &[name, prop] : entityProperties) {
            std::string displayName = name;
            displayName[0] = std::toupper(displayName[0]);
            switch (prop.type) {
                case Entity::Property::Types::Int: {
                    int value = std::stoi(prop.value);
                    if (ImGui::InputInt(displayName.c_str(), &value)) {
                        newProperties[name].value = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Float: {
                    float value = std::stof(prop.value);
                    if (ImGui::InputFloat(displayName.c_str(), &value)) {
                        newProperties[name].value = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Double: {
                    double value = std::stod(prop.value);
                    if (ImGui::InputDouble(displayName.c_str(), &value)) {
                        newProperties[name].value = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Bool: {
                    bool value = std::stoi(prop.value);
                    if (ImGui::Checkbox(displayName.c_str(), &value)) {
                        newProperties[name].value = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Vec2f: {
                    Vec2f value = Vec2f::FromString(prop.value);
                    if (ImGui::InputFloat2(displayName.c_str(), &value.x)) {
                        newProperties[name].value = value.ToString();
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Hidden:
                    break;
                default: {
                    char buffer[256];
                    strncpy_s(buffer, prop.value.c_str(), sizeof(buffer));
                    buffer[sizeof(buffer) - 1] = '\0';
                    if (ImGui::InputText(displayName.c_str(), buffer, sizeof(buffer))) {
                        newProperties[name].value = buffer;
                        changed = true;
                    }
                }
            }
        }

        if (changed) m_entity->Init(newProperties);
    }
}

Entity *Properties::GetEntity() const { return m_entity; }
void Properties::SetEntity(Entity *entity) { m_entity = entity; }

void Properties::Clear() { m_entity = nullptr; }
}  // namespace Editor
}  // namespace Cleave