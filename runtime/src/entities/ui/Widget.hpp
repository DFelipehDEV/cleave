#pragma once
#include "entities/Entity.hpp"

namespace Cleave {
class Widget : public Entity {
public:
    Widget(Transform transform, Vec2f size)
        : Entity(transform), m_size(size) {};
    ~Widget() = default;

    Vec2f GetSize() const;
    void SetSize(const Vec2f size);

private:
    Vec2f m_size;
};
}  // namespace Cleave