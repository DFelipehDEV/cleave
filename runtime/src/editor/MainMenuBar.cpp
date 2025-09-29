#include "MainMenuBar.hpp"

#include <imgui.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "Log.hpp"

#include "platform/FileDialog.hpp"
#include "platform/MessageBox.hpp"

#include "editor/EditorContext.hpp"
#include "scene/JsonSceneSerializer.hpp"

namespace Cleave {
namespace Editor {
void MainMenuBar::OnRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Folder", "Ctrl+O")) {
                std::filesystem::path folder = FileDialog::OpenFolder();
                if (folder != "") m_explorer->SetDirectory(folder);
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                auto currentGameView = m_editor->GetCurrentGameView();
                if (currentGameView && currentGameView->GetScene()) {
                    Scene* scene = currentGameView->GetScene();
                    if (!scene->GetPath().empty()) {
                        JsonSceneSerializer::Save(scene->GetPath(), scene);
                        LOG_INFO("Saved scene: " << scene->GetPath());
                    }
                }
            }

            if (ImGui::MenuItem("Save as..")) {
                LOG_INFO(FileDialog::SaveFile("Project file (*.json)\0*.json\0"));
            }

            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Hierarchy", nullptr, m_editor->IsHierarchyVisible())) {
                m_editor->SetHierarchyVisible(!m_editor->IsHierarchyVisible());
            }
            if (ImGui::MenuItem("File Explorer", nullptr, m_editor->IsFileExplorerVisible())) {
                m_editor->SetFileExplorerVisible(!m_editor->IsFileExplorerVisible());
            }
            if (ImGui::MenuItem("Game View", nullptr, m_editor->IsGameViewVisible())) {
                m_editor->SetGameViewVisible(!m_editor->IsGameViewVisible());
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About", "F1")) {
                std::string specs =
                    "Cleave\nVersion: 0.0.1\nBuild date: " __DATE__ __TIME__;
                MessageBox::Show("About Cleave", specs.c_str());
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
}  // namespace Editor
}  // namespace Cleave