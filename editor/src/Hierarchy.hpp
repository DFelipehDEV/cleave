#pragma once
#include "entities/Entity.hpp"

class Hierarchy {
public:
	Hierarchy(Entity* root) : m_root(root), m_selectedEntity(root) {};
	~Hierarchy() = default;

	void OnRender();

	Entity* GetRoot();
	void SetRoot(Entity* root);

	Entity* GetSelectedEntity();
private:
	Entity* m_root;
	Entity* m_selectedEntity;
};

