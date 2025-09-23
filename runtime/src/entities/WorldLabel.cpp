#include "entities/WorldLabel.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"

namespace Cleave {
WorldLabel::Entity* WorldLabel::Create() { return new WorldLabel(); }

const std::unordered_map<std::string, Entity::Property> WorldLabel::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["text"] = {m_text, Property::Types::String};
    std::string fontPath;
    if (m_font)
        fontPath = m_font->GetPath();
    else
        fontPath = "";
    properties["font"] = {fontPath, Property::Types::FilePath};
    return properties;
}

void WorldLabel::SetProperty(std::string_view name, const std::string& value) {
    if (name == "text") {
        m_text = value;
    } else if (name == "font") {
        if (!value.empty()) {
            auto font = GET_RESMGR()->Get<Font>(value);
            if (font) {
                m_font = font;
            }
        }
    } else {
        Entity::SetProperty(name, value);
    }
    LOG_INFO("Property " << name << " = " << value);
}

void WorldLabel::OnRender(Renderer* renderer) {
    if (!renderer) return;
    if (m_text.empty() || !m_font || m_font->GetId() == -1) return;

    Vec2f globalPosition = GetTransform().GetWorldPosition();

    auto fontShader = GET_RESMGR()->Get<Shader>("res/shaders/text.vert");
    if (!fontShader) {
        return;
    }
    renderer->SetShader(fontShader->GetHandle());
    renderer->SetDepth(GetDepth());
    renderer->DrawText(
        m_text,
        m_font->GetHandle(),
        globalPosition.x,
        globalPosition.y,
        1.0f,
        Color::White()
    );

    Entity::OnRender(renderer);
}

const std::string WorldLabel::GetText() const { return m_text; }
void WorldLabel::SetText(const std::string& text) { m_text = text; }

std::shared_ptr<Font> WorldLabel::GetFont() const { return m_font; }
void WorldLabel::SetFont(std::shared_ptr<Font> font) { m_font = font; }

Color WorldLabel::GetColor() const { return m_color; }
void WorldLabel::SetColor(const Color& color) { m_color = color; }
} // namespace Cleave