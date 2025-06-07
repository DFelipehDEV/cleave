#include "GameView.hpp"
#include <algorithm>
#include <imgui.h>
#include "scene/JsonSceneSerializer.hpp"

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

GameView::GameView(std::shared_ptr<Scene> scene) : m_scene(std::move(scene)) {
    CreateGameFramebuffer(512, 288, m_frameBuffer, m_frameBufferTexture);
}

void GameView::OnUpdate() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel != 0.0f) {
        ImVec2 panelMin = ImGui::GetCursorScreenPos();
        ImVec2 mousePosInPanel = ImVec2(io.MousePos.x - panelMin.x, io.MousePos.y - panelMin.y);

        float oldZoom = m_zoom;
        m_zoom /= 1.0f - io.MouseWheel * 0.1f;
        SetZoom(m_zoom); // this is to clamp the value
    
        m_cameraPos.x += (mousePosInPanel.x / oldZoom) - (mousePosInPanel.x / m_zoom);
        m_cameraPos.y += (mousePosInPanel.y / oldZoom) - (mousePosInPanel.y / m_zoom);
    }
}

void GameView::OnRender(Renderer* renderer) {
    if (ImGui::Button("Save Scene")) {
        JsonSceneSerializer::Save("SavedScene.json", GetScene());
    }
    if (ImGui::Button("Load Scene")) {
        auto scene = JsonSceneSerializer::Load("SavedScene.json");
        if (scene) {
            m_scene->Clear();
            m_scene->SetRoot(scene->ReleaseRoot());
        }
        scene.reset();
    }
    static ImVec2 lastSize = { 512, 288 };
    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    if (contentSize.x != lastSize.x || contentSize.y != lastSize.y) {
        lastSize = contentSize;
    
        // Recreate the framebuffer with the new size
        glDeleteTextures(1, &m_frameBufferTexture);
        glDeleteFramebuffers(1, &m_frameBuffer);
        CreateGameFramebuffer(static_cast<uint32_t>(contentSize.x), static_cast<uint32_t>(contentSize.y), m_frameBuffer, m_frameBufferTexture);
        
        renderer->SetViewPort(Rect4f(0.0f, 0.0f, contentSize.x, contentSize.y));
    }

    float width = contentSize.x / m_zoom;
    float height = contentSize.y / m_zoom;
    
    renderer->SetProjection(glm::ortho(
        m_cameraPos.x, m_cameraPos.x + width,
        m_cameraPos.y + height, m_cameraPos.y,
        -1.0f, 1.0f
    ));

    // Render scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    renderer->ClearColor(100, 149, 237, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    m_scene->Render(renderer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    ImGui::Image((ImTextureID)(intptr_t)m_frameBufferTexture, contentSize, ImVec2(0, 1), ImVec2(1, 0));
}

float GameView::GetZoom() const { return m_zoom; }
void GameView::SetZoom(float zoom) { m_zoom = std::clamp(zoom, 0.1f, 3.0f); }

Scene* GameView::GetScene() const { return m_scene.get(); }
void GameView::SetScene(std::shared_ptr<Scene> scene) { m_scene = std::move(scene); }