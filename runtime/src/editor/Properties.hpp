#pragma once
#include "entities/Entity.hpp"

class Properties {
public:
    Properties() = default;
    ~Properties() = default;
    void OnRender();

    Entity* GetEntity() const;
    void SetEntity(Entity* entity);

    void Clear();
private:
    Entity* m_entity;
};