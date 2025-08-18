#pragma once
#include <filesystem>
#include <string>

#include "editor/GameView.hpp"

namespace Cleave {
namespace Editor {
class FileExplorer {
public:
    FileExplorer(const std::filesystem::path& directory, GameView* gameView)
        : m_directory(directory), m_gameView(gameView) {}
    ~FileExplorer() = default;

    void ShowDirectory(std::filesystem::path dir);

    void OnRender();

    std::filesystem::path GetDirectory() const;
    void SetDirectory(std::filesystem::path directory);

private:
    GameView* m_gameView;
    std::filesystem::path m_directory;
    std::filesystem::path m_selectedDirectory;
};
}  // namespace Editor
}  // namespace Cleave