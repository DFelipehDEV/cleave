#include "AnimatedSprite.hpp"

#include <math.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"

void Cleave::AnimatedSprite::OnTick(float deltaTime) {
    if (!m_playing || !GetTexture()) return;
    
    m_time += deltaTime;
    
    while (m_time >= m_frameDuration) {
        m_time -= m_frameDuration;
        m_currentFrame++;
        
        int totalFrames = m_framesX * m_framesY;
        if (m_currentFrame >= totalFrames) {
            if (m_loop) {
                m_currentFrame = 0;
            } else {
                m_currentFrame = totalFrames - 1;
                Stop();
            }
        }
    }
}

void Cleave::AnimatedSprite::OnRender(Renderer* renderer) {
    if (!GetTexture() || !renderer) return;
    
    int frameX = (m_currentFrame % m_framesX) * m_frameWidth;
    int frameY = (m_currentFrame / m_framesX) * m_frameHeight;
    
    Vec2f globalPosition = GetTransform().GetWorldPosition();
    float rotation = GetTransform().GetWorldRotation();
    Vec2f scale = GetTransform().GetWorldScale();
    
    Vec2f offset = Vec2f(m_frameWidth * 0.5f, m_frameHeight * 0.5f);
    
    Matrix4 model;
    model.Translate(globalPosition);
    model.Translate(-offset);
    model.Rotate(rotation);
    model.Scale(scale);
    
    auto resourceManager = GET_RESMGR();
    auto shader = resourceManager->Get<Shader>("shaders/sprite.vert");
    if (!shader) return;
    
    shader->Use();
    shader->SetUniformInt("tex", 0);
    shader->SetUniformMatrix4("projection", glm::value_ptr(renderer->GetProjection()));
    shader->SetUniformMatrix4("model", (float*)model.m);
    
    float texWidth = static_cast<float>(GetTexture()->GetWidth());
    float texHeight = static_cast<float>(GetTexture()->GetHeight());
    float u0 = frameX / texWidth;
    float v0 = frameY / texHeight;
    float u1 = (frameX + m_frameWidth) / texWidth;
    float v1 = (frameY + m_frameHeight) / texHeight;
    
    GetTexture()->Bind();
    renderer->DrawQuad(0, 0, static_cast<float>(m_frameWidth), 
                      static_cast<float>(m_frameHeight), u0, v0, u1, v1);
    
    Entity::OnRender(renderer);
}

void Cleave::AnimatedSprite::Play(float fps, bool loop) {
    m_frameDuration = 1.0f / fps;
    m_playing = true;
    m_loop = loop;
}

void Cleave::AnimatedSprite::Stop() {
    m_playing = false;
}

void Cleave::AnimatedSprite::SetFrame(int frame) {
    int totalFrames = m_framesX * m_framesY;
    if (frame >= 0 && frame < totalFrames) {
        m_currentFrame = frame;
    }
}