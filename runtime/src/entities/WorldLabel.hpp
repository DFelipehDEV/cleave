#pragma once

#include "Entity.hpp"
#include "resources/Font.hpp"

namespace Cleave {
class WorldLabel : public Entity {
public:
    WorldLabel(Transform transform = Transform(), const std::string& text = "", std::shared_ptr<Font> font = nullptr)
        : Entity(transform), m_text(text), m_font(font) {}

    void OnRender(Renderer* renderer) override;
    
    static const char* GetTypeName() { return "cleave::WorldLabel"; }
    const PropertyMap GetProperties() const override;
    void SetProperty(std::string_view name, const std::string& value) override;

    static Entity* Create();

    const std::string GetText() const;
    void SetText(const std::string& text);

    std::shared_ptr<Font> GetFont() const;
    void SetFont(std::shared_ptr<Font> font);

    Color GetColor() const;
    void SetColor(const Color& color);
private:
    std::string m_text;
    std::shared_ptr<Font> m_font;
    Color m_color = Color::White();
};
} // namespace Cleave