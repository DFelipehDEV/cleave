#pragma once
#include <memory>
#include <string>

namespace Cleave {
class Scene;
class Properties;

class JsonSceneSerializer {
public:
    static std::shared_ptr<Scene> Load(const std::string& path);
    static bool Save(const std::string& path, Scene* scene);
};
}  // namespace Cleave