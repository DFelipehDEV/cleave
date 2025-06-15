#pragma once
#include "editor/FileExplorer.hpp"

namespace Cleave {
namespace Editor {
class MainMenuBar {
public:
    MainMenuBar(FileExplorer* explorer) : m_explorer(explorer) {}
    ~MainMenuBar() = default;

    void OnRender();

private:
    FileExplorer* m_explorer;
};
}  // namespace Editor
}  // namespace Cleave