#pragma once
#include <vector>
#include "../Vec2.hpp"
#include "../rendering/Renderer.hpp"
#include "../Transform.hpp"

class Entity {
public:
    Entity(Transform transform) : m_transform(transform) {}
    virtual ~Entity();

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    std::string GetName() const;
    void SetName(std::string name);

    Transform& GetTransform();
    void SetTransform(Transform& transform);

    Entity* GetParent() const;
    void SetParent(Entity* parent);

    const std::vector<Entity*>& GetChildren() const;
    void AddChild(Entity* child);
    void RemoveChild(Entity* child);

    Entity* GetChild(const std::string& name, bool recursive = false) const;

    Entity* GetRoot();
private:
    std::string m_name = "";
    Transform m_transform;
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
};