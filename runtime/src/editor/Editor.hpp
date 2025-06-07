#pragma once
#include <memory>

#include "scene/Scene.hpp"
#include "editor/MainMenuBar.hpp"
#include "editor/Hierarchy.hpp"
#include "editor/Properties.hpp"
#include "editor/GameView.hpp"
#include "editor/FileExplorer.hpp"

class Editor {
public:
	Editor(Window* window);
	~Editor() = default;
	void Run(Renderer* renderer);

	Scene* GetScene();
	Hierarchy* GetHierarchy();
	FileExplorer* GetFileExplorer();
	Properties* GetProperties();
	GameView* GetGameView();
private:
	Window* m_window;
	std::shared_ptr<MainMenuBar> m_menuBar;
	std::shared_ptr<Scene> m_scene;
	std::shared_ptr<Hierarchy> m_hierarchy;
	std::shared_ptr<FileExplorer> m_fileExplorer;
	std::shared_ptr<Properties> m_properties;
	std::shared_ptr<GameView> m_gameView;
};