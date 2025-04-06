#pragma once
#include <vector>
#include "../Vec2.hpp"
#include "../rendering/Renderer.hpp"

class Entity {
public:
    Entity(Vec2f position = {0.0f, 0.0f}) : m_position(position) {}
    virtual ~Entity();

    virtual void OnTick(float deltaTime);
    virtual void OnRender(Renderer* renderer);

    std::string GetName() const;
    void SetName(std::string& name);

    Vec2f GetPosition() const;
    void SetPosition(Vec2f& position);

    Vec2f GetGlobalPosition() const;

    float* GetTransform() {
        // Rebuild matrix each time (simple approach)
        static float matrix[16] = {
            m_scale * cos(m_rotation), -m_scale * sin(m_rotation), 0, m_position.x,
            m_scale * sin(m_rotation),  m_scale * cos(m_rotation), 0, m_position.y,
            0,                     0,                      1, 0,
            0,                     0,                      0, 1
        };
        return matrix;
    }

    Entity* GetParent() const;
    void SetParent(Entity* parent);

    const std::vector<Entity*>& GetChildren() const;
    void AddChild(Entity* child);
    void RemoveChild(Entity* child);

    Entity* GetChild(const std::string& name, bool recursive = false) const;

    Entity* GetRoot();
private:
    std::string m_name = "";
    Vec2f m_position;
    float m_rotation = 0.0f;
    float m_scale = 1.0f;
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
};