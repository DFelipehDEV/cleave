#pragma once
#include "resources/ResourceManager.hpp"
class Services {
private:
    static ResourceManager* s_resourceManager;

public:
    static void Provide(ResourceManager* resourceManager);
    static ResourceManager* GetResourceManager();
};