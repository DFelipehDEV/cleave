#include "scene/JsonSceneSerializer.hpp"

#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>

#include "EntityRegistry.hpp"
#include "entities/Entity.hpp"
#include "scene/Scene.hpp"
#include "Log.hpp"

namespace Cleave {
std::shared_ptr<Scene> JsonSceneSerializer::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open: " << path);
        return nullptr;
    }

    nlohmann::json json;
    try {
        file >> json;
    } catch (const std::exception& e) {
        LOG_ERROR("JSON parse error: " << e.what());
        return nullptr;
    }

    std::vector<
        std::pair<Entity*, std::unordered_map<std::string, Entity::Property>>>
        pendingInits;
    auto scene = std::make_shared<Scene>(std::make_unique<Entity>(Transform()));

    std::function<void(const nlohmann::json&, Entity*)> deserialize;
    deserialize = [&](const nlohmann::json& jsonData, Entity* parent) {
        auto typeIt = Registry::GetAllTypes().find(jsonData["type"]);
        if (typeIt != Registry::GetAllTypes().end()) {
            std::unique_ptr<Entity> entity = Registry::CreateEntity(typeIt->first);

            std::unordered_map<std::string, Entity::Property> props;
            for (auto& [key, val] : jsonData.items()) {
                if (key != "children") props[key].value = val.get<std::string>();
            }
            entity->Init(props);

            Entity* rawPtr = entity.get();
            parent->AddChild(std::move(entity));

            if (jsonData.contains("children")) {
                for (auto& childJson : jsonData["children"]) {
                    deserialize(childJson, rawPtr);
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
        LOG_ERROR("Scene loading failed: " << e.what());
        return nullptr;
    }

    return scene;
}

bool JsonSceneSerializer::Save(const std::string& path, Scene* scene) {
    std::ofstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for writing: " << path);
        return false;
    }

    nlohmann::json json;

    std::function<void(Entity*, nlohmann::json&)> serialize =
        [&serialize](Entity* entity, nlohmann::json& jsonOut) {
            const auto& properties = entity->GetProperties();
            for (auto& [key, prop] : properties) {
                jsonOut[key] = prop.value;
            }

            const auto& children = entity->GetChildren();
            if (!children.empty()) {
                nlohmann::json childrenArray = nlohmann::json::array();
                for (const auto& child : children) {
                    nlohmann::json childJson;
                    serialize(child.get(), childJson);
                    childrenArray.push_back(childJson);
                }
                jsonOut["children"] = childrenArray;
            }
        };

    serialize(scene->GetRoot(), json);

    file << json.dump(4);

    return true;
}
}  // namespace Cleave