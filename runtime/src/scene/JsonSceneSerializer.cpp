#include "scene/JsonSceneSerializer.hpp"
#include <fstream>
#include <iostream>
#include <functional>
#include <nlohmann/json.hpp>

#include "entities/Entity.hpp"
#include "scene/Scene.hpp"
#include "EntityRegistry.hpp"

 std::shared_ptr<Scene> JsonSceneSerializer::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << path << std::endl;
        return nullptr;
    }

    nlohmann::json json;
    try {
        file >> json;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return nullptr;
    }

    auto scene = std::make_shared<Scene>(std::make_unique<Entity>(Transform(), "root"));

    std::vector<std::pair<Entity*, std::unordered_map<std::string, Entity::Property>>> pendingInits;
    std::function<void(const nlohmann::json&, Entity*)> deserialize = [&](const nlohmann::json& jsonData, Entity* parent) {
        std::unordered_map<std::string, Entity::Property> properties;
        for (const auto& [key, value] : jsonData.items()) {
            if (key != "children") {
                properties[key].value = value.get<std::string>();
            }
        }
        auto factory = Registry::GetAllTypes().find(jsonData["type"]);
        if (factory != Registry::GetAllTypes().end()) {
            Entity* entity = Registry::CreateEntity(factory->first);

            pendingInits.push_back({entity, properties});
            
            parent->AddChild(entity);

            if (jsonData.contains("children")) {
                for (const auto& child : jsonData["children"]) {
                    deserialize(child, entity);
                }
            }
        }
    };

    try {
        if (json.contains("children")) {
            for (const auto& child : json["children"]) {
                deserialize(child, scene->GetRoot());
            }
        }

        for (auto& [entity, properties] : pendingInits) {
            entity->Init(properties);
        }
    } catch (const std::exception& e) {
        std::cerr << "Scene loading failed: " << e.what() << std::endl;
        return nullptr;
    }

    return scene;
}

bool JsonSceneSerializer::Save(const std::string& path, Scene* scene) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << path << std::endl;
        return false;
    }

    nlohmann::json json;

    std::function<void(Entity*, nlohmann::json&)> serialize = [&serialize](Entity* entity, nlohmann::json& jsonOut) {
        const auto& properties = entity->GetProperties();
        for (const auto& [key, prop] : properties) {
            jsonOut[key] = prop.value;
        }

        const auto& children = entity->GetChildren();
        if (!children.empty()) {
            nlohmann::json childrenArray = nlohmann::json::array();
            for (const auto& child : children) {
                nlohmann::json childJson;
                serialize(child, childJson);
                childrenArray.push_back(childJson);
            }
            jsonOut["children"] = childrenArray;
        }
    };

    serialize(scene->GetRoot(), json);

    file << json.dump(4);

    return true;
}
