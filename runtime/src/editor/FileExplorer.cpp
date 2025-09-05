#include "FileExplorer.hpp"

#include <imgui.h>

#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

#include "editor/EditorContext.hpp"
#include "resources/ResourceManager.hpp"

namespace Cleave {
namespace Editor {
void FileExplorer::ShowDirectory(std::filesystem::path dir) {
    for (const auto& file : std::filesystem::directory_iterator(dir)) {
        const auto& path = file.path();
        auto fileName = file.path().filename().string();
        if (std::filesystem::is_regular_file(file.path())) {
            bool opened = ImGui::TreeNodeEx(
                fileName.c_str(),
                ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) &&
                !ImGui::IsItemToggledOpen()) {
                m_selectedDirectory = path;
            }
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
                if (path.extension() == ".jscn") {
                    std::shared_ptr<Scene> scene = std::dynamic_pointer_cast<Scene>(SceneLoader().Load(path.generic_string(), GET_RESMGR()));
                    if (scene) {
                        std::cout << "Opening scene: " << scene->GetPath() << std::endl;
                        m_editorContext->AddGameView(std::make_shared<GameView>(scene, m_editorContext->GetProperties()));
                    }
                    scene.reset();
                }
            }
        } else {
            bool opened = ImGui::TreeNodeEx(
                fileName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow |
                                      ImGuiTreeNodeFlags_OpenOnDoubleClick);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) &&
                !ImGui::IsItemToggledOpen()) {
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
    if (ImGui::BeginPopupContextWindow("FileExplorerContextMenu",
                                       ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("New File")) {
            std::ofstream ofs(m_selectedDirectory / "New File.txt");
            ofs << "this is some text in the new file\n";
            ofs.close();
        }

        if (ImGui::MenuItem("New Folder")) {
            std::filesystem::create_directory(m_selectedDirectory /
                                              "New Folder");
        }

// TODO: add other platforms
#ifdef _WIN32
        if (ImGui::MenuItem("Reveal in File Explorer")) {
            const std::string path = m_selectedDirectory.string();
            std::wstring wideFilePath(path.begin(), path.end());
            ShellExecuteW(nullptr, L"open", L"explorer.exe",
                          (L"/select," + wideFilePath).c_str(), nullptr,
                          SW_SHOWNORMAL);
        }
#endif

        if (ImGui::MenuItem("Rename")) {
        }

        if (ImGui::MenuItem("Delete")) {
            std::filesystem::remove(m_selectedDirectory);
        }

        if (ImGui::MenuItem("Copy Relative Path")) {
            std::string relPath = std::filesystem::relative(
                m_selectedDirectory, m_directory)
                                      .string();
            #ifdef _WIN32
            std::replace(relPath.begin(), relPath.end(), '\\', '/');
            #endif
            ImGui::SetClipboardText(relPath.c_str());
        }

        if (ImGui::MenuItem("Copy Path")) {
            std::string path = m_selectedDirectory.string();
            #ifdef _WIN32
            std::replace(path.begin(), path.end(), '\\', '/');
            #endif
            ImGui::SetClipboardText(path.c_str());
        }

        ImGui::EndPopup();
    }

    ImGui::TextUnformatted(m_directory.filename().string().c_str());
    ShowDirectory(m_directory);
}

std::filesystem::path FileExplorer::GetDirectory() const { return m_directory; }
void FileExplorer::SetDirectory(std::filesystem::path directory) {
    m_directory = directory;
    m_selectedDirectory = "";
}
}  // namespace Editor
}  // namespace Cleave