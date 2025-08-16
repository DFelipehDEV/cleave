#pragma once
#include <memory>

#include "editor/FileExplorer.hpp"
#include "editor/GameView.hpp"
#include "editor/Hierarchy.hpp"
#include "editor/MainMenuBar.hpp"
#include "editor/Properties.hpp"
#include "scene/Scene.hpp"

namespace Cleave {
namespace Editor {
class EditorContext {
public:
    EditorContext(Window* window);
    ~EditorContext() = default;
    void Run(Renderer* renderer);

    MainMenuBar* GetMenuBar() const;
    Scene* GetScene();
    Hierarchy* GetHierarchy();
    FileExplorer* GetFileExplorer();
    Properties* GetProperties();
    GameView* GetGameView();
    
    bool IsHierarchyVisible() const;
    void SetHierarchyVisible(bool visible);

    bool IsFileExplorerVisible() const;
    void SetFileExplorerVisible(bool visible);

    bool IsGameViewVisible() const;
    void SetGameViewVisible(bool visible);
    
private:
    Window* m_window;
    std::shared_ptr<MainMenuBar> m_menuBar;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Hierarchy> m_hierarchy;
    std::shared_ptr<FileExplorer> m_fileExplorer;
    std::shared_ptr<Properties> m_properties;
    std::shared_ptr<GameView> m_gameView;
    bool m_hierarchyVisible = true;
    bool m_fileExplorerVisible = true;
    bool m_gameViewVisible = true;
};
}  // namespace Editor
}  // namespace Cleave