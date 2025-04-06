#pragma once
#include <string>

class Resource {
public:
    virtual ~Resource() = default;
    
    std::string GetName() const;
    void SetName(const std::string name);
private:
    std::string m_name;
};