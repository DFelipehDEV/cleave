#pragma once
#include <string>

class Resource {
public:
    virtual ~Resource() = default;

    virtual std::string GetTypeName() const = 0;

    std::string GetPath() const;
    void SetPath(const std::string path);

    uint32_t GetId() const;
    void SetId(uint32_t id);

protected:
    uint32_t m_id = 0;
    std::string m_path;
};