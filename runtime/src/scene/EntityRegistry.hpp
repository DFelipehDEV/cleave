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
    requires std::derived_from<T, Entity> && std::default_initializable<T>
    static void RegisterType() {
        GetFactories()[T::GetTypeName()] = []() {
            return std::make_unique<T>();
        };
    }

    static std::unique_ptr<Entity> CreateEntity(const std::string& typeName) {
        auto& factories = GetFactories();
        auto it = factories.find(typeName);
        return it != factories.end() ? it->second() : nullptr;
    }

    static const std::unordered_map<std::string, EntityFactory>& GetAllTypes() { return GetFactories(); }
};
}  // namespace Cleave