#pragma once
#include <vector>
#include <unordered_map>
#include "../Vec2.hpp"
#include "../rendering/Renderer.hpp"
#include "../Transform.hpp"

typedef uint64_t EntityID;
static EntityID NEXT_ENTITY_ID = 0;

class Entity {
public:
    Entity(Transform transform = Transform()) : m_transform(transform), m_id(NEXT_ENTITY_ID++) {}
    virtual ~Entity();

    virtual void Init(const std::unordered_map<std::string, std::string>& properties);

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    virtual std::string GetType() const { return "cleave::Entity"; }

    EntityID GetID() const;
    void SetID(EntityID id);

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
    EntityID m_id;
    std::string m_name = "";
    Transform m_transform;
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
};