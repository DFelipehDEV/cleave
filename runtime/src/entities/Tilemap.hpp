#pragma once

#include "entities/Entity.hpp"
#include "rendering/Material.hpp"

namespace Cleave {
struct Tile {
    int16_t index = -1;
    bool flippedX = false;
    bool flippedY = false;
    Vec2f uv0, uv1;
};

class Tilemap : public Entity {
public:
    Tilemap() = default;
    Tilemap(Transform transform) : Entity(transform) {};

    void OnRender(Renderer* renderer) override;

    static const char* GetTypeName() { return "cleave::Tilemap"; }
    const PropertyMap GetProperties() const override;
    void SetProperty(const std::string_view name, const std::string& value) override;

    static Entity* Create();

    void Resize(uint32_t width, uint32_t height);
    uint32_t GetWidth() const;
    void SetWidth(uint32_t width);
    uint32_t GetHeight() const;
    void SetHeight(uint32_t height);

    const std::vector<Tile>& GetTiles() const;
    void SetTiles(const std::vector<Tile>& tiles);
    const Tile& GetTile(uint32_t x, uint32_t y) const;
    void SetTile(uint32_t x, uint32_t y, const Tile& tile);
private:
    Material m_material;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::vector<Tile> m_tiles;
};
} // namespace Cleave
