#include "Properties.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <filesystem>
#include <functional>

#include "platform/FileDialog.hpp"

#include "imgui.h"

namespace Cleave {
namespace Editor {
void Properties::OnRender(Scene* scene) {
    if (!scene) return;

    Entity* entity = scene->GetRoot()->GetChild(m_entityId, true);
    if (!entity) return;

    EntityId id = entity->GetId();
    auto entityProperties = entity->GetProperties();

    if (!entityProperties.empty()) {
        bool changed = false;
        for (auto &[name, prop] : entityProperties) {
            std::string displayName = name;
            std::string newValue = prop.value;
            if (!displayName.empty())
                displayName[0] = std::toupper(displayName[0]);
            switch (prop.type) {
                case Entity::Property::Types::Int: {
                    int value = std::stoi(prop.value);
                    if (ImGui::InputInt(displayName.c_str(), &value)) {
                        newValue = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Float: {
                    float value = std::stof(prop.value);
                    if (ImGui::InputFloat(displayName.c_str(), &value)) {
                        newValue = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Double: {
                    double value = std::stod(prop.value);
                    if (ImGui::InputDouble(displayName.c_str(), &value)) {
                        newValue = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Bool: {
                    bool value = std::stoi(prop.value);
                    if (ImGui::Checkbox(displayName.c_str(), &value)) {
                        newValue = std::to_string(value);
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Vec2f: {
                    Vec2f value = Vec2f::FromString(prop.value);
                    if (ImGui::InputFloat2(displayName.c_str(), &value.x)) {
                        newValue = value.ToString();
                        changed = true;
                    }
                    break;
                }

                case Entity::Property::Types::Hidden:
                    break;

                case Entity::Property::Types::FilePath:
                    if (ImGui::Button(("Pick '" + name + "'").c_str())) {
                        ImGui::OpenPopup("LoadPopup");
                    } 

                    if (ImGui::BeginPopup("LoadPopup")) {
                        ImGui::BeginChild("FileList", ImVec2(300, 400), true);
                        for (const auto& entry : std::filesystem::recursive_directory_iterator("res/")) {
                            if (entry.is_regular_file()) {
                                std::string path = entry.path().generic_string();
                                if (ImGui::Selectable(path.substr(4).c_str())) {
                                    newValue = path;
                                    changed = true;
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        ImGui::EndChild();
                        ImGui::EndPopup();
                    }
                    break;
                    case Entity::Property::Types::EntityId: {
                        std::vector<std::string> options;
                        std::vector<EntityId> ids;

                        std::function<void(Entity*)> gather = [&](Entity* e) {
                            if (!e) return;
                            ids.push_back(e->GetId());
                            options.push_back(e->GetId() + " (" + e->GetTypeName() + ")");
                            for (auto& c : e->GetChildren())
                                gather(c.get());
                        };
                        gather(scene->GetRoot());

                        if (!ids.empty()) {
                            int currentIndex = 0;
                            auto it = std::find(ids.begin(), ids.end(), prop.value);
                            if (it != ids.end())
                                currentIndex = static_cast<int>(std::distance(ids.begin(), it));

                            std::vector<const char*> options_cstr;
                            for (const auto& s : options)
                                options_cstr.push_back(s.c_str());

                            if (ImGui::Combo(displayName.c_str(), &currentIndex, options_cstr.data(), (int)options_cstr.size())) {
                                newValue = ids[currentIndex];
                                changed = true;
                            }
                        }
                        break;
                    }
                default: {
                    char buffer[256];
                    strncpy_s(buffer, prop.value.c_str(), sizeof(buffer));
                    buffer[sizeof(buffer) - 1] = '\0';
                    if (ImGui::InputText(displayName.c_str(), buffer, sizeof(buffer))) {
                        newValue = buffer;
                        changed = true;
                    }
                }
            }
            if (changed) entity->SetProperty(name, newValue);
        }
    }
}

EntityId Properties::GetEntityId() const { return m_entityId; }
void Properties::SetEntityId(EntityId id) { m_entityId = id; }

void Properties::Clear() { m_entityId = ""; }
}  // namespace Editor
}  // namespace Cleave