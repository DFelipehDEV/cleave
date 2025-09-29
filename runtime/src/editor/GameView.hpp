#pragma once
#include <stdint.h>

#include <memory>

#include "rendering/Renderer.hpp"
#include "scene/Scene.hpp"
#include "editor/Properties.hpp"

namespace Cleave {
namespace Editor {
class GameView {
public:
    GameView(std::shared_ptr<Scene> scene, std::shared_ptr<Properties> properties);
    void OnUpdate();
    void OnRender(Renderer* renderer);

    float GetZoom() const;
    void SetZoom(float zoom);

    Scene* GetScene() const;
    void SetScene(std::shared_ptr<Scene> scene);

    bool IsGridEnabled() const;
    void SetGridEnabled(bool enabled);

    int GetGridSize() const;
    void SetGridSize(int size);
private:
    std::shared_ptr<Scene> m_runtimeScene;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Properties> m_properties;
    uint32_t m_frameBuffer;
    uint32_t m_frameBufferTexture;

    float m_zoom = 1.0f;
    Vec2f m_cameraPos = {0, 0};
    bool m_gridEnabled = true;
    int m_gridSize = 32;
    bool m_playing = false;
};
}  // namespace Editor
}  // namespace Cleave