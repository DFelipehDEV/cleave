#pragma once
#include <memory>

#include "Scene.hpp"
#include "Hierarchy.hpp"

class Editor {
public:
	Editor();
	~Editor() = default;

	Scene* GetScene();
	Hierarchy* GetHierarchy();
private:
	std::shared_ptr<Scene> m_scene;
	std::shared_ptr<Hierarchy> m_hierarchy;
};