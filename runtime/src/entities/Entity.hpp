#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "math/Transform.hpp"
#include "math/Vec2.hpp"
#include "rendering/Renderer.hpp"
#include "UUID.hpp"

typedef std::string EntityId;

namespace Cleave {
class Entity {
public:
    Entity(Transform transform = Transform())
        : m_transform(transform),  m_id(GenerateUUID()) {}

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;

    Entity(Entity&& other) noexcept
        : m_id(other.m_id),
          m_name(std::move(other.m_name)),
          m_transform(std::move(other.m_transform)),
          m_parent(other.GetParent()),
          m_children(std::move(other.m_children)) {
        for (const auto& child : m_children) {
            if (child) {
                child->SetParent(this);
            }
        }
        other.m_id = "";
        other.SetParent(nullptr);
    }

    Entity& operator=(Entity&& other) noexcept {
        if (this != &other) {
            for (const auto& child : m_children) {
                if (child) {
                    child->SetParent(nullptr);
                }
            }
            m_children.clear();

            m_id = other.m_id;
            m_transform = std::move(other.m_transform);
            SetParent(other.GetParent());
            m_children = std::move(other.m_children);

            for (const auto& child : m_children) {
                if (child) {
                    child->SetParent(this);
                }
            }
            other.m_id = "";
            other.SetParent(nullptr);
        }
        return *this;
    }

    virtual ~Entity() = default;

    struct Property {
        enum class Types {
            Hidden,
            Int,
            Float,
            Double,
            Bool,
            String,
            Vec2f,
            FilePath,
            EntityId,
        };
        std::string value;
        Types type = Types::Hidden;
    };
    using PropertyMap = std::unordered_map<std::string, Property>;

    void Init(const PropertyMap& properties);

    void Tick(float deltaTime);
    void Render(Renderer* renderer);

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    static const char* GetTypeName() { return "cleave::Entity"; }

    virtual const PropertyMap GetProperties() const;
    virtual void SetProperty(const std::string_view name, const std::string& value);

    static Entity* Create();

    EntityId GetId() const;
    void SetId(EntityId id);

    const std::string& GetName() const;
    void SetName(const std::string& name);

    Transform& GetTransform();
    void SetTransform(Transform& transform);

    int GetDepth();
    void SetDepth(int depth);

    Entity* GetParent() const;
    void SetParent(Entity* parent);

    std::vector<std::unique_ptr<Entity>>& GetChildren();
    void AddChild(std::unique_ptr<Entity> child);
    void RemoveChild(Entity* child);

    Entity* GetChild(EntityId id, bool recursive = false) const;

    Entity* GetRoot();

private:
    EntityId m_id;
    std::string m_name;
    Transform m_transform;
    Entity* m_parent = nullptr;
    std::vector<std::unique_ptr<Entity>> m_children;
    int m_depth = 0;
    bool m_active = true;
    bool m_visible = true;
};
}  // namespace Cleave