#include "MainMenuBar.hpp"
#include <imgui.h>
#include <cstdlib>

void MainMenuBar::OnRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project")) { 
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) { 
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
            }
            if (ImGui::MenuItem("Save as..")) { 
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