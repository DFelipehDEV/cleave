#pragma once
#include <memory>
#include <string>

namespace Cleave {
class ResourceManager;

class Resource {
public:
    virtual ~Resource() = default;

    virtual std::string_view GetTypeName() const = 0;

    const std::string& GetPath() const;
    void SetPath(const std::string& path);

    uint32_t GetId() const;
    void SetId(uint32_t id);

protected:
    uint32_t m_id = 0;
    std::string m_path;
};

class ResourceLoader {
public:
    virtual ~ResourceLoader() = default;
    virtual std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) = 0;
    virtual bool CanLoad(const std::string_view extension) const = 0;
};

}  // namespace Cleave