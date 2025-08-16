#pragma once
#include "editor/FileExplorer.hpp"

#include <functional>

namespace Cleave {
namespace Editor {
class EditorContext;
class MainMenuBar {
public:
    MainMenuBar(EditorContext* editor, FileExplorer* explorer) : m_editor(editor), m_explorer(explorer) {}
    ~MainMenuBar() = default;

    void OnRender();

private:
    EditorContext* m_editor;
    FileExplorer* m_explorer;
};
}  // namespace Editor
}  // namespace Cleave