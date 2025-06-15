#include "Widget.hpp"

namespace Cleave {
Vec2f Widget::GetSize() const { return m_size; }
void Widget::SetSize(const Vec2f size) { m_size = size; }
}  // namespace Cleave