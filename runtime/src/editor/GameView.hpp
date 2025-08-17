#pragma once
#include <stdint.h>

#include <memory>

#include "rendering/Renderer.hpp"
#include "scene/Scene.hpp"

namespace Cleave {
namespace Editor {
class GameView {
public:
    GameView(std::shared_ptr<Scene> scene);
    void OnUpdate();
    void OnRender(Renderer* renderer);

    float GetZoom() const;
    void SetZoom(float zoom);

    Scene* GetScene() const;
    void SetScene(std::shared_ptr<Scene> scene);

    bool IsGridEnabled() const { return m_gridEnabled; }
    void SetGridEnabled(bool enabled) { m_gridEnabled = enabled; }

    int GetGridSize() const { return m_gridSize; }
    void SetGridSize(int size) { m_gridSize = std::max(size, 1); }
private:
    std::shared_ptr<Scene> m_scene;
    uint32_t m_frameBuffer;
    uint32_t m_frameBufferTexture;

    float m_zoom = 1.0f;
    Vec2f m_cameraPos = {0, 0};
    bool m_gridEnabled = true;
    int m_gridSize = 32;
};
}  // namespace Editor
}  // namespace Cleave