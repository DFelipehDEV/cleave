#pragma once
#include <functional>

#include "entities/Entity.hpp"

namespace Cleave {
class Registry {
private:
    using EntityFactory = std::function<std::unique_ptr<Entity>()>;
    static std::unordered_map<std::string, EntityFactory>& GetFactories() {
        static std::unordered_map<std::string, EntityFactory> factories;
        return factories;
    }

public:
    template <typename T>
    static void RegisterType() {
        static_assert(std::is_base_of_v<Entity, T>,
                      "T must derive from Entity");
        static_assert(std::is_default_constructible_v<T>,
                      "T must be default constructible");

        GetFactories()[T::GetTypeName()] = []() {
            return std::make_unique<T>();
        };
    }

    static std::unique_ptr<Entity> CreateEntity(const std::string& typeName) {
        auto& factories = GetFactories();
        auto it = factories.find(typeName);
        return it != factories.end() ? it->second() : nullptr;
    }

    static const std::unordered_map<std::string, EntityFactory>& GetAllTypes() {
        return GetFactories();
    }
};
}  // namespace Cleave