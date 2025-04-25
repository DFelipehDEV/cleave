#pragma once
#include <stdint.h>

#include "rendering/Renderer.hpp"
#include "Scene.hpp"

class GameView {
public:
    GameView(Scene* scene);
    void OnUpdate();
    void OnRender(Renderer* renderer);

    float GetZoom() const;
    void SetZoom(float zoom);
private:
    Scene* m_scene;
    uint32_t m_frameBuffer;
    uint32_t m_frameBufferTexture;

    float m_zoom = 1.0f;
    Vec2f m_cameraPos = {0, 0};
};