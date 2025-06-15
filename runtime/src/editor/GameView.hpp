#pragma once
#include <stdint.h>

#include <memory>

#include "rendering/Renderer.hpp"
#include "scene/Scene.hpp"

class GameView {
public:
    GameView(std::shared_ptr<Scene> scene);
    void OnUpdate();
    void OnRender(Renderer* renderer);

    float GetZoom() const;
    void SetZoom(float zoom);

    Scene* GetScene() const;
    void SetScene(std::shared_ptr<Scene> scene);

private:
    std::shared_ptr<Scene> m_scene;
    uint32_t m_frameBuffer;
    uint32_t m_frameBufferTexture;

    float m_zoom = 1.0f;
    Vec2f m_cameraPos = {0, 0};
};