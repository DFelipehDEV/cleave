#pragma once
#include <memory>

#include "Scene.hpp"
#include "Hierarchy.hpp"
#include "Properties.hpp"
#include "GameView.hpp"

class Editor {
public:
	Editor();
	~Editor() = default;
	void OnRender(Renderer* renderer);

	Scene* GetScene();
	Hierarchy* GetHierarchy();
	Properties* GetProperties();
	GameView* GetGameView();
private:
	std::shared_ptr<Scene> m_scene;
	std::shared_ptr<Hierarchy> m_hierarchy;
	std::shared_ptr<Properties> m_properties;
	std::shared_ptr<GameView> m_gameView;
};