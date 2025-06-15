#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Cleave {
class Services {
private:
    static std::unordered_map<std::string, std::shared_ptr<void>>&
    GetServices() {
        static std::unordered_map<std::string, std::shared_ptr<void>> services;
        return services;
    }

public:
    template <typename T>
    static void Provide(const std::string& name, std::shared_ptr<T> service) {
        GetServices()[name] = std::static_pointer_cast<void>(service);
    }

    template <typename T>
    static void Provide(const std::string& name, T* service) {
        GetServices()[name] = std::shared_ptr<void>(service, [](void*) {});
    }

    template <typename T>
    static T* Get(const std::string& name) {
        auto& services = GetServices();
        auto it = services.find(name);
        if (it == services.end()) {
            throw std::runtime_error("Service '" + name + "' not found");
        }
        return static_cast<T*>(it->second.get());
    }

    template <typename T>
    static std::shared_ptr<T> GetShared(const std::string& name) {
        auto& services = GetServices();
        auto it = services.find(name);
        if (it == services.end()) {
            throw std::runtime_error("Service '" + name + "' not found");
        }
        return std::static_pointer_cast<T>(it->second);
    }

    static bool IsProvided(const std::string& name) {
        return GetServices().find(name) != GetServices().end();
    }

    static void Remove(const std::string& name) { GetServices().erase(name); }

    static void Clear() { GetServices().clear(); }
};
}  // namespace Cleave