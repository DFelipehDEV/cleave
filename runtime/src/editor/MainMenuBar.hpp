#pragma once
#include "editor/FileExplorer.hpp"

class MainMenuBar {
public:
    MainMenuBar(FileExplorer* explorer) : m_explorer(explorer) {}
    ~MainMenuBar() = default;

    void OnRender();
private:
    FileExplorer* m_explorer;
};