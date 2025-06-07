#include "FileExplorer.hpp"
#include <imgui.h>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

void FileExplorer::ShowDirectory(std::filesystem::path dir) {
    for (const auto& file : std::filesystem::directory_iterator(dir)) {
        const auto& path = file.path();
        auto fileName = file.path().filename().string();
        if (std::filesystem::is_regular_file(file.path())) { 
            bool opened = ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !ImGui::IsItemToggledOpen()) {
                m_selectedDirectory = path;
            }
        } else {
            bool opened = ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !ImGui::IsItemToggledOpen()) {
                m_selectedDirectory = path;
            }

            if (opened) {
                ShowDirectory(path);
                ImGui::TreePop();
            }
        }
    }
}
void FileExplorer::OnRender() {

    if (ImGui::BeginPopupContextWindow("FileExplorerContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("New File")) {
            std::ofstream ofs(m_selectedDirectory / "New File.txt");
            ofs << "this is some text in the new file\n"; 
            ofs.close();
        }

        if (ImGui::MenuItem("New Folder")) {
            std::filesystem::create_directory(m_selectedDirectory / "New Folder");
        }

        //TODO: add other platforms
        #ifdef _WIN32
        if (ImGui::MenuItem("Reveal in File Explorer")) {
            const std::string path = m_selectedDirectory.string();
            std::wstring wideFilePath(path.begin(), path.end());
            ShellExecuteW(nullptr, L"open", L"explorer.exe", (L"/select," + wideFilePath).c_str(), nullptr, SW_SHOWNORMAL);
        }
        #endif

        if (ImGui::MenuItem("Rename")) {

        }

        if (ImGui::MenuItem("Delete")) {
            std::filesystem::remove(m_selectedDirectory);
        }

        ImGui::EndPopup();
    }

    ImGui::TextUnformatted(m_directory.filename().string().c_str());
    ShowDirectory(m_directory);
}