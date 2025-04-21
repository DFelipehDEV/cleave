#include "Editor.hpp"

Editor::Editor() {
	m_scene = std::make_shared<Scene>();
	Entity* root = new Entity(Transform({ 0, 0 }));
	root->SetName("Root");
	m_scene->SetRoot(root);

	m_hierarchy = std::make_shared<Hierarchy>(GetScene()->GetRoot());
}

Scene* Editor::GetScene() { return m_scene.get(); }

Hierarchy* Editor::GetHierarchy() { return m_hierarchy.get(); }
