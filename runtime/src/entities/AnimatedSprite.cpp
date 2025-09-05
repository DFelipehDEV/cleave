#include "AnimatedSprite.hpp"

#include <math.h>
#include <algorithm>
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "entities/Entity.hpp"

namespace Cleave {
void AnimatedSprite::Init(const std::unordered_map<std::string, Property> properties) {
    Sprite::Init(properties);
    if (properties.find("playing") != properties.end())
        m_playing = std::stoi(properties.at("playing").value);
    if (properties.find("frameWidth") != properties.end())
        m_frameWidth = std::stoi(properties.at("frameWidth").value);
    if (properties.find("frameHeight") != properties.end())
        m_frameHeight = std::stoi(properties.at("frameHeight").value);
    
    m_framesX = GetTexture() ? GetTexture()->GetWidth() / m_frameWidth : 0;
    m_framesY = GetTexture() ? GetTexture()->GetHeight() / m_frameHeight : 0;
}

const std::unordered_map<std::string, Entity::Property> AnimatedSprite::GetProperties() const {
    auto properties = Sprite::GetProperties();
    properties["type"] = {GetTypeName(), Entity::Property::Types::Hidden};
    properties["playing"] = {std::to_string(IsPlaying()), Entity::Property::Types::Bool};
    properties["frameWidth"] = {std::to_string(m_frameWidth), Entity::Property::Types::Int};
    properties["frameHeight"] = {std::to_string(m_frameHeight), Entity::Property::Types::Int};
    return properties;
}

Entity* AnimatedSprite::Create() { return new AnimatedSprite(); }

void AnimatedSprite::OnTick(float deltaTime) {
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

void AnimatedSprite::OnRender(Renderer* renderer) {
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
    auto shader = resourceManager->Get<Shader>("res/shaders/sprite.vert");
    if (!shader) return;
    
    float texWidth = static_cast<float>(GetTexture()->GetWidth());
    float texHeight = static_cast<float>(GetTexture()->GetHeight());
    float u0 = frameX / texWidth;
    float v0 = frameY / texHeight;
    float u1 = (frameX + m_frameWidth) / texWidth;
    float v1 = (frameY + m_frameHeight) / texHeight;
    
    renderer->AddRenderCommand(new RenderQuadCommand(
        0,
        0, 
        static_cast<float>(m_frameWidth),          
        static_cast<float>(m_frameHeight), 
        GetTexture()->GetTextureId(), 
        shader->GetShaderId(), 
        model,
        renderer->GetProjection(), 
        GetDepth(),
        u0,
        v0, 
        u1, 
        v1 
    ));
    
    Entity::OnRender(renderer);
}

void AnimatedSprite::Play(float fps, bool loop) {
    m_frameDuration = 1.0f / fps;
    m_playing = true;
    m_loop = loop;
}

void AnimatedSprite::Stop() {
    m_playing = false;
}

void AnimatedSprite::SetFrame(int frame) {
    int totalFrames = m_framesX * m_framesY;
    if (frame >= 0 && frame < totalFrames) {
        m_currentFrame = frame;
    }
}
} // namespace Cleave