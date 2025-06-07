#pragma once
#include <functional>

#include "entities/Entity.hpp"

//TODO: Move to services
struct TypeRegistry {
    const std::string type;
    const std::function<Entity*()> factory;
};

struct Registry {
    static void RegisterType(const TypeRegistry& reg) {
        typeMap()[reg.type] = reg.factory;
    }

    static std::unordered_map<std::string, std::function<Entity*()>>& typeMap() {
        static std::unordered_map<std::string, std::function<Entity*()>> map;
        return map;
    }
};