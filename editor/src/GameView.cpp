#include "GameView.hpp"
#include "imgui.h"

void CreateGameFramebuffer(int width, int height, uint32_t& framebuffer, uint32_t& framebufferTexture) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GameView::GameView(Scene* scene) : m_scene(scene) {
    CreateGameFramebuffer(512, 288, m_frameBuffer, m_frameBufferTexture);
}

void GameView::OnRender(Renderer* renderer) {
    static ImVec2 lastSize = { 512, 288 };
    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    if (contentSize.x != lastSize.x || contentSize.y != lastSize.y) {
        lastSize = contentSize;
    
        // Recreate the framebuffer with the new size
        glDeleteTextures(1, &m_frameBufferTexture);
        glDeleteFramebuffers(1, &m_frameBuffer);
        CreateGameFramebuffer(static_cast<uint32_t>(contentSize.x), static_cast<uint32_t>(contentSize.y), m_frameBuffer, m_frameBufferTexture);
        
        renderer->SetViewPort(Rect4f(0.0f, 0.0f, contentSize.x, contentSize.y));
        renderer->SetProjection(glm::ortho(
            0.0f, contentSize.x,
            contentSize.y, 0.0f,
            -1.0f, 1.0f
        ));
    }

    // Render scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    renderer->ClearColor(100, 149, 237, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    m_scene->Render(renderer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    ImGui::Image((ImTextureID)(intptr_t)m_frameBufferTexture, contentSize, ImVec2(0, 1), ImVec2(1, 0));
}