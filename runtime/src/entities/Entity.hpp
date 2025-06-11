#pragma once
#include <vector>
#include <unordered_map>
#include "math/Vec2.hpp"
#include "rendering/Renderer.hpp"
#include "math/Transform.hpp"

typedef uint64_t EntityID;
static EntityID NEXT_ENTITY_ID = 0;

class Entity {
public:
    Entity(Transform transform = Transform(), const std::string& name = "") : m_transform(transform), m_name(name), m_id(NEXT_ENTITY_ID++) {}
    Entity(const Entity& other) 
        : m_id(other.m_id)
        , m_name(other.m_name)
        , m_transform(other.m_transform)
        , m_parent(other.m_parent)
        , m_children(other.m_children) { }
    virtual ~Entity();

    struct Property {
        enum class Types {
            Int,
            Float,
            Double,
            Bool,
            String,
            Vec2f,
            Hidden,
        };
        std::string value;
        Types type;
    };

    virtual void Init(const std::unordered_map<std::string, Property> properties);

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    virtual std::string GetTypeName() const { return "cleave::Entity"; }

    virtual const std::unordered_map<std::string, Property> GetProperties() const; 

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
    Entity* GetChild(EntityID id, bool recursive = false) const;

    Entity* GetRoot();
private:
    EntityID m_id;
    std::string m_name = "";
    Transform m_transform;
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
};