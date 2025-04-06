#pragma once
#include "entities/Entity.hpp"

class Scene {
public:
    Scene() : m_root(nullptr) {}
    ~Scene();
    
    Entity* GetRoot() const;
    void SetRoot(Entity* root);
    
    void Tick();
    void Render();
private:
    Entity* m_root;
};