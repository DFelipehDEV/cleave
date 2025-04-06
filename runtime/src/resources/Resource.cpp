#include "Resource.hpp"

std::string Resource::GetName() const {
    return m_name;
}

void Resource::SetName(const std::string name) {
    m_name = name;
}