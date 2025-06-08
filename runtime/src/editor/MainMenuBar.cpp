#include "MainMenuBar.hpp"
#include <imgui.h>
#include "platform/FileDialog.hpp"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>

void MainMenuBar::OnRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Folder", "Ctrl+O")) { 
                std::cout << FileDialog::OpenFolder() << std::endl;
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
            }

            if (ImGui::MenuItem("Save as..")) { 
                std::cout << FileDialog::SaveFile("Project file (*.json)\0*.json\0") << std::endl;
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
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About", "F1")) { 
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}