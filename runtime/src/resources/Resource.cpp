#include "Resource.hpp"

namespace Cleave {
uint32_t Resource::GetId() const { return m_id; }
void Resource::SetId(const uint32_t id) { m_id = id; }

const std::string& Resource::GetPath() const { return m_path; }
void Resource::SetPath(const std::string& path) { m_path = path; }
}  // namespace Cleave