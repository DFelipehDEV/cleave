#pragma once
#include <stdint.h>

#include "rendering/Renderer.hpp"
#include "Scene.hpp"

class GameView {
public:
    GameView(Scene* scene);
    void OnRender(Renderer* renderer);
private:
    Scene* m_scene;
    uint32_t m_frameBuffer;
    uint32_t m_frameBufferTexture;
};