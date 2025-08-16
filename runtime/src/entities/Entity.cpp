#include "Entity.hpp"

#include <iostream>

namespace Cleave {
Entity::~Entity() {}

void Entity::Init(const std::unordered_map<std::string, Property> properties) {
    if (properties.find("id") != properties.end())
        SetId(std::stoi(properties.at("id").value));
    if (properties.find("name") != properties.end())
        SetName(properties.at("name").value);

    if (properties.find("position") != properties.end())
        m_transform.SetPosition(
            Vec2f::FromString(properties.at("position").value));
    if (properties.find("scale") != properties.end())
        m_transform.SetScale(Vec2f::FromString(properties.at("scale").value));
    if (properties.find("rotation") != properties.end())
        m_transform.SetRotationDegrees(
            std::stof(properties.at("rotation").value));
}

void Entity::OnTick(float deltaTime) {
    for (const auto& child : m_children) {
        child->OnTick(deltaTime);
    }
}

void Entity::OnRender(Renderer* renderer) {
    for (const auto& child : m_children) {
        child->OnRender(renderer);
    }
}

const std::unordered_map<std::string, Entity::Property> Entity::GetProperties()
    const {
    std::unordered_map<std::string, Property> properties;
    properties["type"] = {GetTypeName(), Entity::Property::Types::Hidden};
    properties["id"] = {std::to_string(m_id), Entity::Property::Types::Hidden};

    properties["name"] = {m_name, Entity::Property::Types::String};
    properties["position"] = {m_transform.GetPosition().ToString(),
                              Entity::Property::Types::Vec2f};
    properties["scale"] = {m_transform.GetScale().ToString(),
                           Entity::Property::Types::Vec2f};
    properties["rotation"] = {std::to_string(m_transform.GetRotation()),
                              Entity::Property::Types::Float};
    return properties;
}

Entity* Entity::Create() { return new Entity(); }

EntityID Entity::GetId() const { return m_id; }
void Entity::SetId(EntityID id) {
    m_id = id;
    if (id >= NEXT_ENTITY_ID) {
        NEXT_ENTITY_ID = id + 1;
    }
}

std::string Entity::GetName() const { return m_name; }
void Entity::SetName(std::string_view name) { m_name = name; }

Transform& Entity::GetTransform() { return m_transform; }
void Entity::SetTransform(Transform& transform) { m_transform = transform; }

Entity* Entity::GetParent() const { return m_parent; }
void Entity::SetParent(Entity* parent) {
    m_parent = parent;
    if (parent) {
        m_transform.SetParent(&parent->m_transform);
    } else {
        m_transform.SetParent(nullptr);
    }
}

std::vector<Entity*> Entity::GetChildren() const {
    std::vector<Entity*> children;
    children.reserve(m_children.size());
    for (const auto& child_ptr : m_children) {
        children.push_back(child_ptr.get());
    }
    return children;
}

void Entity::AddChild(std::unique_ptr<Entity> child) {
    if (!child) return;

    // Remove from previous parent
    if (child->m_parent) {
        child->m_parent = nullptr;
    }

    child->SetParent(this);
    m_children.push_back(std::move(child));
}

void Entity::RemoveChild(Entity* child) {
    m_children.erase(
        std::remove_if(m_children.begin(), m_children.end(),
                       [child](const std::unique_ptr<Entity>& ptr) {
                           return ptr.get() == child;
                       }),
        m_children.end());
}

Entity* Entity::GetChild(std::string_view name, bool recursive) const {
    for (const auto& child : m_children) {
        if (child->m_name == name) {
            return child.get();
        }
        if (recursive) {
            if (Entity* found = child->GetChild(name, true)) {
                return found;
            }
        }
    }
    return nullptr;
}

Entity* Entity::GetChild(EntityID id, bool recursive) const {
    for (const auto& child : m_children) {
        if (child->m_id == id) {
            return child.get();
        }
        if (recursive) {
            if (Entity* found = child->GetChild(id, true)) {
                return found;
            }
        }
    }
    return nullptr;
}

Entity* Entity::GetRoot() {
    Entity* current = this;
    while (current->GetParent()) {
        current = current->GetParent();
    }
    return current;
}
}  // namespace Cleave