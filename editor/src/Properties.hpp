#pragma once
#include "entities/Entity.hpp"

class Properties {
public:
    struct EditorEntityProperties {
        std::string headerPath;
        std::unordered_map<std::string, std::string> properties;
        std::unordered_map<std::string, std::string> values;
    };
    Properties() = default;
    ~Properties() = default;
    void OnRender();

    Entity* GetEntity() const;
    void SetEntity(Entity* entity);

    std::unordered_map<std::string, std::string> ParseEntityHeader(const std::string& headerPath) const;
private:
    Entity* m_entity;
    std::unordered_map<EntityID, EditorEntityProperties> m_editorProperties;
};