#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "math/Transform.hpp"
#include "math/Vec2.hpp"
#include "rendering/Renderer.hpp"


typedef uint64_t EntityID;
static EntityID NEXT_ENTITY_ID = 0;

namespace Cleave {
class Entity {
public:
    Entity(Transform transform = Transform(), const std::string& name = "")
        : m_transform(transform), m_name(name), m_id(NEXT_ENTITY_ID++) {}

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;

    Entity(Entity&& other) noexcept
        : m_id(other.m_id),
          m_name(std::move(other.m_name)),
          m_transform(std::move(other.m_transform)),
          m_parent(other.m_parent),
          m_children(std::move(other.m_children)) {
        for (const auto& child : m_children) {
            if (child) {
                child->m_parent = this;
            }
        }
        other.m_id = 0;
        other.m_parent = nullptr;
    }

    Entity& operator=(Entity&& other) noexcept {
        if (this != &other) {
            for (const auto& child : m_children) {
                if (child) {
                    child->m_parent = nullptr;
                }
            }
            m_children.clear();

            m_id = other.m_id;
            m_name = std::move(other.m_name);
            m_transform = std::move(other.m_transform);
            m_parent = other.m_parent;
            m_children = std::move(other.m_children);

            for (const auto& child : m_children) {
                if (child) {
                    child->m_parent = this;
                }
            }
            other.m_id = 0;
            other.m_parent = nullptr;
        }
        return *this;
    }

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
        Types type = Types::Hidden;
    };

    virtual void Init(
        const std::unordered_map<std::string, Property> properties);

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    static const char* GetTypeName() { return "cleave::Entity"; }

    virtual const std::unordered_map<std::string, Property> GetProperties()
        const;

    static Entity* Create();

    EntityID GetID() const;
    void SetID(EntityID id);

    std::string GetName() const;
    void SetName(std::string_view name);

    Transform& GetTransform();
    void SetTransform(Transform& transform);

    Entity* GetParent() const;
    void SetParent(Entity* parent);

    std::vector<Entity*> GetChildren() const;
    void AddChild(std::unique_ptr<Entity> child);
    void RemoveChild(Entity* child);

    Entity* GetChild(std::string_view name, bool recursive = false) const;
    Entity* GetChild(EntityID id, bool recursive = false) const;

    Entity* GetRoot();

private:
    EntityID m_id;
    std::string m_name = "";
    Transform m_transform;
    Entity* m_parent = nullptr;
    std::vector<std::unique_ptr<Entity>> m_children;
};
}  // namespace Cleave