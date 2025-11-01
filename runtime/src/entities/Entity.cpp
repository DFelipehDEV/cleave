#include "Entity.hpp"

namespace Cleave {
void Entity::Init(const PropertyMap& properties) {
    for (const auto& [name, prop] : properties) {
        SetProperty(name, prop.value);
    }
}

void Entity::Tick(float deltaTime) {
    OnTick(deltaTime);

    for (const auto& child : m_children) {
        child->Tick(deltaTime);
    }
}

void Entity::Render(Renderer* renderer) {
    OnRender(renderer);

    for (const auto& child : m_children) {
        child->Render(renderer);
    }
}

void Entity::OnTick(float deltaTime) {}
void Entity::OnRender(Renderer* renderer) {}

const Entity::PropertyMap Entity::GetProperties() const {
    PropertyMap properties;
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["id"] = {m_id, Property::Types::Hidden};
    properties["name"] = {m_name, Property::Types::String};
    properties["position"] = {m_transform.GetPosition().ToString(), Property::Types::Vec2f};
    properties["scale"] = {m_transform.GetScale().ToString(), Property::Types::Vec2f};
    properties["rotation"] = {std::to_string(m_transform.GetRotationDegrees()), Property::Types::Float};
    properties["depth"] = {std::to_string(m_depth), Property::Types::Int};
    return properties;
}

void Entity::SetProperty(const std::string_view name, const std::string& value) {
    if (name == "id") {
        SetId(value);
    } else if (name == "name") {
        SetName(value);
    } else if (name == "position") {
        m_transform.SetPosition(Vec2f::FromString(value));
    } else if (name == "scale") {
        m_transform.SetScale(Vec2f::FromString(value));
    } else if (name == "rotation") {
        m_transform.SetRotationDegrees(std::stof(value));
    } else if (name == "depth") {
        SetDepth(std::stoi(value));
    }
}

Entity* Entity::Create() { return new Entity(); }

EntityId Entity::GetId() const { return m_id; }
void Entity::SetId(EntityId id) { m_id = id; }

const std::string& Entity::GetName() const { return m_name; }
void Entity::SetName(const std::string& name) { m_name = name; }

Transform& Entity::GetTransform() { return m_transform; }
void Entity::SetTransform(Transform& transform) { m_transform = transform; }

Vec2f Entity::GetPosition() { return m_transform.GetPosition(); }
void Entity::SetPosition(const Vec2f& position) { m_transform.SetPosition(position); }

Vec2f Entity::GetScale() { return m_transform.GetScale(); }
void Entity::SetScale(const Vec2f& scale) { m_transform.SetScale(scale); }

float Entity::GetRotation() { return m_transform.GetRotationDegrees(); }
void Entity::SetRotation(const float rotation) { m_transform.SetRotationDegrees(rotation); }

int Entity::GetDepth() { return m_depth; }
void Entity::SetDepth(int depth) { m_depth = depth; }

Entity* Entity::GetParent() const { return m_parent; }
void Entity::SetParent(Entity* parent) {
    m_parent = parent;
    if (parent) {
        m_transform.SetParent(&parent->m_transform);
    } else {
        m_transform.SetParent(nullptr);
    }
}

std::vector<std::unique_ptr<Entity>>& Entity::GetChildren() {
    return m_children;
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

Entity* Entity::GetChild(EntityId id, bool recursive) const {
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