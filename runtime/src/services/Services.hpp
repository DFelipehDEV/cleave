#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Log.hpp"
#include "services/Service.hpp"

namespace Cleave {
class Services {
private:
    static std::unordered_map<std::string, std::shared_ptr<void>>& GetServices() {
        static std::unordered_map<std::string, std::shared_ptr<void>> services;
        return services;
    }

public:
    template <typename T>
    requires std::derived_from<T, Service>
    static void Provide(std::shared_ptr<T> service) {
        GetServices()[service->GetTypeName()] = std::static_pointer_cast<void>(service);
    }

    template <typename T>
    requires std::derived_from<T, Service>
    static void Provide(T* service) {
        GetServices()[service->GetTypeName()] = std::shared_ptr<void>(service, [](void*) {});
    }

    template <typename T>
    requires std::derived_from<T, Service>
    static T* Get() {
        auto& services = GetServices();
        std::string typeName = T::GetTypeName();
        auto it = services.find(typeName);
        if (it == services.end()) {
            std::string error = "Service '" + typeName + "' not found";
            LOG_ERROR(error);
            throw std::runtime_error(error);
        }
        return static_cast<T*>(it->second.get());
    }

    template <typename T>
    requires std::derived_from<T, Service>
    static std::shared_ptr<T> GetShared() {
        auto& services = GetServices();
        std::string typeName = T::GetTypeName();
        auto it = services.find(typeName);
        if (it == services.end()) {
            std::string error = "Service '" + typeName + "' not found";
            LOG_ERROR(error);
            throw std::runtime_error(error);
        }
        return std::static_pointer_cast<T>(it->second);
    }

    template <typename T>
    requires std::derived_from<T, Service>
    static bool IsProvided() {
        std::string typeName = T::GetTypeName();
        return GetServices().find(typeName) != GetServices().end();
    }

    template <typename T>
    requires std::derived_from<T, Service>
    static void Remove() {
        std::string typeName = T::GetTypeName();
        GetServices().erase(typeName);
    }

    static void Clear() { GetServices().clear(); }
};
}  // namespace Cleave