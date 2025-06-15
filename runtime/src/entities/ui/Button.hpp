#pragma once
#include "Widget.hpp"

class Button : public Widget {
public:
    Button(Transform transform, std::string label)
        : Widget(transform, {32, 16}), m_label(label) {}
    ~Button() = default;

    void OnRender(Renderer* renderer) override;

private:
    std::string m_label;
};