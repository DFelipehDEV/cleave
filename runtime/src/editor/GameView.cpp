#include "GameView.hpp"

#include <imgui.h>

#include <algorithm>

#include "services/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "scene/JsonSceneSerializer.hpp"


namespace Cleave {
namespace Editor {
void CreateGameFramebuffer(int width, int height, uint32_t& framebuffer,
                           uint32_t& framebufferTexture) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           framebufferTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GameView::GameView(std::shared_ptr<Scene> scene, std::shared_ptr<Properties> properties) : m_scene(std::move(scene)), m_properties(std::move(properties)) {
    CreateGameFramebuffer(512, 288, m_frameBuffer, m_frameBufferTexture);
}

void GameView::OnUpdate() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel != 0.0f) {
        ImVec2 panelMin = ImGui::GetCursorScreenPos();
        ImVec2 mousePosInPanel =
            ImVec2(io.MousePos.x - panelMin.x, io.MousePos.y - panelMin.y);

        float oldZoom = m_zoom;
        m_zoom /= 1.0f - io.MouseWheel * 0.1f;
        SetZoom(m_zoom);  // this is to clamp the value

        m_cameraPos.x +=
            (mousePosInPanel.x / oldZoom) - (mousePosInPanel.x / m_zoom);
        m_cameraPos.y +=
            (mousePosInPanel.y / oldZoom) - (mousePosInPanel.y / m_zoom);
    }
}

void GameView::OnRender(Renderer* renderer) {
    ImGui::Text(m_scene->GetPath() != "" ? m_scene->GetPath().c_str() : "No Scene Loaded");
    if (ImGui::BeginTable("Toolbar", 3, ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableNextColumn();
        if (ImGui::Button(m_playing ? "Stop" : "Play")) {
            auto scenePath = m_scene->GetPath();
            m_playing = !m_playing;
            if (m_playing) {
                JsonSceneSerializer::Save(scenePath, m_scene.get());
            } else {
                m_properties->Clear();
                m_scene = std::dynamic_pointer_cast<Scene>(SceneLoader().Load(scenePath, GET_RESMGR()));     
            }
        }
        ImGui::TableNextColumn();
        ImGui::Checkbox("Show Grid", &m_gridEnabled);
        ImGui::InputInt("Grid Size", &m_gridSize);
        ImGui::EndTable();
    }
    static ImVec2 lastSize = {512, 288};
    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    if (contentSize.x != lastSize.x || contentSize.y != lastSize.y) {
        lastSize = contentSize;

        // Recreate the framebuffer with the new size
        glDeleteTextures(1, &m_frameBufferTexture);
        glDeleteFramebuffers(1, &m_frameBuffer);
        CreateGameFramebuffer(static_cast<uint32_t>(contentSize.x),
                              static_cast<uint32_t>(contentSize.y),
                              m_frameBuffer, m_frameBufferTexture);

        renderer->SetViewPort(Rect4f(0.0f, 0.0f, contentSize.x, contentSize.y));
    }

    float width = contentSize.x / m_zoom;
    float height = contentSize.y / m_zoom;

    renderer->SetProjection(Matrix4::Ortho(m_cameraPos.x, m_cameraPos.x + width,
                                       m_cameraPos.y + height, m_cameraPos.y,
                                       -1.0f, 1.0f));

    // Render scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    renderer->ClearColor(Color(100, 149, 237, 255));
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_gridEnabled) {
        ImVec2 contentSize = ImGui::GetContentRegionAvail();
        float width = contentSize.x / m_zoom;
        float height = contentSize.y / m_zoom;

        Color color = {128, 128, 128, 255};

        int numX = static_cast<int>(width / m_gridSize) + 2;
        int numY = static_cast<int>(height / m_gridSize) + 2;

        for (int i = -1; i < numX; i++) {
            float x = m_cameraPos.x + i * m_gridSize;
            renderer->DrawLine(x, m_cameraPos.y, x, m_cameraPos.y + height,
                               color);
        }
        for (int j = -1; j < numY; j++) {
            float y = m_cameraPos.y + j * m_gridSize;
            renderer->DrawLine(m_cameraPos.x, y, m_cameraPos.x + width, y,
                               color);
        }
    }
    
    m_scene->Render(renderer);
    renderer->RunRenderCommands();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((ImTextureID)(intptr_t)m_frameBufferTexture, contentSize,
                 ImVec2(0, 1), ImVec2(1, 0));
    ImVec2 imageMin = ImGui::GetItemRectMin();
}

float GameView::GetZoom() const { return m_zoom; }
void GameView::SetZoom(float zoom) { m_zoom = std::clamp(zoom, 0.1f, 3.0f); }

Scene* GameView::GetScene() const { return m_scene.get(); }
void GameView::SetScene(std::shared_ptr<Scene> scene) {
    m_scene = std::move(scene);
}

bool GameView::IsGridEnabled() const { return m_gridEnabled; }
void GameView::SetGridEnabled(bool enabled) { m_gridEnabled = enabled; }

int GameView::GetGridSize() const { return m_gridSize; }
void GameView::SetGridSize(int size) { m_gridSize = std::max(size, 1); }
}  // namespace Editor
}  // namespace Cleave