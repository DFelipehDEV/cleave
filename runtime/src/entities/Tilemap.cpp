#include "entities/Tilemap.hpp"
#include "services/ResourceManager.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/RenderCommand.hpp"
#include "rendering/Material.hpp"
#include "math/Rect4.hpp"
#include "Log.hpp"
#include "Tilemap.hpp"

namespace Cleave {
Entity* Tilemap::Create() { return new Tilemap(); }

const Entity::PropertyMap Tilemap::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["width"] = {std::to_string(GetWidth()), Property::Types::Int};
    properties["height"] = {std::to_string(GetHeight()), Property::Types::Int};
    properties["texture"] = {m_material.texture ? m_material.texture->GetPath() : "", Entity::Property::Types::FilePath};
    return properties;
}

void Tilemap::SetProperty(const std::string_view name, const std::string& value) {
    if (name == "width") {
        SetWidth(static_cast<uint32_t>(std::stoul(value)));
    } else if (name == "height") {
        SetHeight(static_cast<uint32_t>(std::stoul(value)));
    } else if (name == "texture") {
        if (!value.empty() && GET_RESMGR()->Exists<Texture>(value)) {
            auto tex = GET_RESMGR()->Get<Texture>(value);
            if (tex)
                m_material.texture = tex;
            LOG_INFO("Set tilemap texture to " << tex->GetHandle());
        } else {
            m_material.texture = nullptr;
        }
    }  else {
        Entity::SetProperty(name, value);
    }
}

void Tilemap::OnRender(Renderer* renderer) {
    if (m_tiles.empty()) return;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const Tile& tile = GetTile(x, y);
            if (tile.index < 0) continue;

            Rect4f destRect(
                GetPosition().x + x * GetScale().x,
                GetPosition().y + y * GetScale().y,
                GetScale().x,
                GetScale().y
            );

            RenderQuadCommand cmd(
                destRect,
                1.0f, 1.0f, 0.0f,
                m_material,
                GetDepth(),
                tile.uv0.x, tile.uv0.y,
                tile.uv1.x, tile.uv1.y,
                Color::White()
            );
            renderer->AddRenderCommand(std::make_unique<RenderCommand>(cmd));
        }
    }
}
void Tilemap::Resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    m_tiles.resize(width * height);
}

uint32_t Tilemap::GetWidth() const { return m_width; }
void Tilemap::SetWidth(uint32_t width) { m_width = width; }

uint32_t Tilemap::GetHeight() const { return m_height; }
void Tilemap::SetHeight(uint32_t height) { m_height = height; }

const std::vector<Tile>& Tilemap::GetTiles() const { return m_tiles; }
void Tilemap::SetTiles(const std::vector<Tile>& tiles) { m_tiles = tiles; }
const Tile& Tilemap::GetTile(uint32_t x, uint32_t y) const {
    if (x >= m_width || y >= m_height) {
        LOG_ERROR("Tile coordinates out of bounds: (" << x << ", " << y << ")");
        throw std::exception("Tile coordinates out of bounds");
        return m_tiles[0];
    } 
    return m_tiles[y * m_width + x];
}
void Tilemap::SetTile(uint32_t x, uint32_t y, const Tile& tile) {
    if (x >= m_width || y >= m_height) {
        LOG_ERROR("Tile coordinates out of bounds: (" << x << ", " << y << ")");
        throw std::exception("Tile coordinates out of bounds");
        return;
    }
    m_tiles[static_cast<size_t>(y) * m_width + x] = tile;
}
};  // namespace Cleave