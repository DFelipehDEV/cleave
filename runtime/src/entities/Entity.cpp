#include "Entity.hpp"

Entity::~Entity() {
    if (m_parent) {
        m_parent->RemoveChild(this);
    }
}

void Entity::OnTick(float deltaTime) {
    for (Entity* child : m_children) {
        child->OnTick(deltaTime);
    }
}

void Entity::OnRender(Renderer* renderer) {
    for (Entity* child : m_children) {
        child->OnRender(renderer);
    }
}

std::string Entity::GetName() const {
    return m_name;
}

void Entity::SetName(std::string& name) {
    m_name = name;
}

Vec2f Entity::GetPosition() const {
    return m_position;
}

void Entity::SetPosition(Vec2f& position) {
    m_position = position;
}

Vec2f Entity::GetGlobalPosition() const {
    if (m_parent) return m_position + m_parent->GetGlobalPosition();
    return m_position;
}

Entity* Entity::GetParent() const {
    return m_parent;
}

void Entity::SetParent(Entity* parent) {
    m_parent = parent;
}

const std::vector<Entity*>& Entity::GetChildren() const {
    return m_children;
}

void Entity::AddChild(Entity* child) {
    m_children.push_back(child);
    child->SetParent(this);
}

void Entity::RemoveChild(Entity* child) {
    auto it = std::remove(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it, m_children.end());
        child->m_parent = nullptr;
    }
}

Entity* Entity::GetChild(const std::string& name, bool recursive) const {
    for (Entity* child : m_children) {
        if (child->m_name == name) {
            return child;
        }
        if (recursive) {
            Entity* found = child->GetChild(name, true);
            if (found) {
                return found;
            }
        }
    }
    return nullptr;
}

Entity* Entity::GetRoot()  {
    Entity* current = this;
    while (current->GetParent()) {
        current = current->GetParent();
    }
    return current;
}