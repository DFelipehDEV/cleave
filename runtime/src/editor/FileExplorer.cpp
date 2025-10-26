#include "FileExplorer.hpp"
#include <filesystem>
#include <fstream>

#include <imgui.h>

#include "services/ResourceManager.hpp"
#include "editor/EditorContext.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace Cleave {
namespace Editor {
void FileExplorer::ShowDirectory(const std::filesystem::path& dir) {
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
                        LOG_INFO("Opening scene: " << scene->GetPath());
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

        if (ImGui::MenuItem("New Scene")) {
            std::ofstream ofs(m_selectedDirectory / "New Scene.jscn");
            ofs << R"({
            "id": "0",
            "name": "root",
            "position": "0.000000,0.000000",
            "rotation": "0.000000",
            "scale": "1.000000,1.000000",
            "type": "cleave::Entity"
        })";
            ofs.close();
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
            m_isRenaming = true;
            m_renameBuffer = m_selectedDirectory.filename().string();
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

    if (m_isRenaming) {
        ImGui::OpenPopup("RenameDialog");
    }
    
    if (ImGui::BeginPopupModal("RenameDialog", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Rename:");
        ImGui::Separator();
        
        if (m_isRenaming && ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere();
        }
        
        char buffer[256];
        strncpy(buffer, m_renameBuffer.c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        if (ImGui::InputText("##rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_renameBuffer = buffer;
            if (!m_renameBuffer.empty() && m_renameBuffer != m_selectedDirectory.filename().string()) {
                try {
                    std::filesystem::path newPath = m_selectedDirectory.parent_path() / m_renameBuffer;
                    std::filesystem::rename(m_selectedDirectory, newPath);
                    m_selectedDirectory = newPath;
                } catch (const std::exception& e) {
                    LOG_ERROR("Failed to rename: " << e.what());
                }
            }
            m_isRenaming = false;
            ImGui::CloseCurrentPopup();
        }
        
        m_renameBuffer = buffer;
        
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (!m_renameBuffer.empty() && m_renameBuffer != m_selectedDirectory.filename().string()) {
                try {
                    std::filesystem::path newPath = m_selectedDirectory.parent_path() / m_renameBuffer;
                    std::filesystem::rename(m_selectedDirectory, newPath);
                    m_selectedDirectory = newPath;
                } catch (const std::exception& e) {
                    LOG_ERROR("Failed to rename: " << e.what());
                }
            }
            m_isRenaming = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_isRenaming = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }

    ImGui::TextUnformatted(m_directory.filename().string().c_str());
    ShowDirectory(m_directory);
}

const std::filesystem::path& FileExplorer::GetDirectory() const { return m_directory; }
void FileExplorer::SetDirectory(const std::filesystem::path& directory) {
    m_directory = directory;
    m_selectedDirectory = "";
}
}  // namespace Editor
}  // namespace Cleave