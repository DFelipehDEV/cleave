#include "Entity.hpp"

Entity::~Entity() {
    if (m_parent) {
        m_parent->RemoveChild(this);
    }
}

void Entity::Init(const std::unordered_map<std::string, std::string>& properties) {
    if (properties.find("name") != properties.end()) SetName(properties.at("name"));

    if (properties.find("position") != properties.end()) m_transform.SetPosition(Vec2f::FromString(properties.at("position")));
    if (properties.find("scale") != properties.end()) m_transform.SetScale(Vec2f::FromString(properties.at("scale")));
    if (properties.find("rotation") != properties.end()) m_transform.SetRotation(std::stof(properties.at("rotation")));
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

EntityID Entity::GetID() const { return m_id; }
void Entity::SetID(EntityID id) {
    m_id = id;
    if (id >= NEXT_ENTITY_ID) {
        NEXT_ENTITY_ID = id + 1;
    }
}

std::string Entity::GetName() const { return m_name; }
void Entity::SetName(std::string name) { m_name = name; }

Transform& Entity::GetTransform() { return m_transform; }
void Entity::SetTransform(Transform& transform) { m_transform = transform; }

Entity* Entity::GetParent() const { return m_parent; }
void Entity::SetParent(Entity* parent) {
    if (parent) {
        parent->AddChild(this);
    } else if (m_parent) {
        m_parent->RemoveChild(this);
    }
}

const std::vector<Entity*>& Entity::GetChildren() const { return m_children; }

void Entity::AddChild(Entity* child) {
    // Remove from previous parent
    if (child->m_parent) {
        child->m_parent->RemoveChild(child);
    }

    m_children.push_back(child);
    child->m_parent = this;
    child->m_transform.SetParent(&this->m_transform);
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