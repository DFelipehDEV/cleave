#include "Services.hpp"
#include <iostream>

ResourceManager* Services::s_resourceManager = nullptr;

void Services::Provide(ResourceManager* resourceManager) {
    s_resourceManager = resourceManager;
}

ResourceManager* Services::GetResourceManager() {
    if (s_resourceManager == nullptr) {
        std::cerr << "ResourceManager not provided to Services" << std::endl;
    }
    return s_resourceManager;
}