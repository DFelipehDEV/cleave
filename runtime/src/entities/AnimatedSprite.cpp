#include "AnimatedSprite.hpp"

#include <math.h>
#include <algorithm>
#include "services/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "entities/Entity.hpp"

namespace Cleave {
const std::unordered_map<std::string, Entity::Property> AnimatedSprite::GetProperties() const {
    auto properties = Sprite::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    properties["playing"] = {std::to_string(IsPlaying()), Property::Types::Bool};
    properties["frameSize"] = {m_frameSize.ToString(), Property::Types::Vec2f};
    properties["frameCount"] = {std::to_string(m_frameCount), Property::Types::Int};
    properties["frameDuration"] = {std::to_string(m_frameDuration), Property::Types::Float};
    properties["loop"] = {std::to_string(m_loop), Property::Types::Bool};
    return properties;
}

void AnimatedSprite::SetProperty(std::string_view name, const std::string& value) {
    if (name == "playing") {
        m_playing = std::stoi(value);
    } else if (name == "frameSize") {
        m_frameSize = Vec2i::FromString(value);
    } else if (name == "frameCount") {
        m_frameCount = std::stoi(value); 
    } else if (name == "frameDuration") {
        m_frameDuration = std::stof(value);
    } else if (name == "loop") {
        m_loop = std::stoi(value);
    } else {
        Sprite::SetProperty(name, value);
    }
}

Entity* AnimatedSprite::Create() { return new AnimatedSprite(); }

void AnimatedSprite::OnTick(float deltaTime) {
    if (!m_playing || !GetTexture() || m_frameDuration <= 0) return;
    
    m_time += deltaTime;
    
    while (m_time >= m_frameDuration) {
        m_time -= m_frameDuration;
        m_frame++;
        
        if (m_frame >= m_frameCount) {
            if (m_loop) {
                m_frame = 0;
            } else {
                m_frame = m_frameCount - 1;
                Stop();
            }
        }
    }
}

void AnimatedSprite::OnRender(Renderer* renderer) {
    if (!GetTexture() || !renderer) return;
    
    Vec2f globalPosition = GetTransform().GetWorldPosition();
    float rotation = GetTransform().GetWorldRotation();
    Vec2f scale = GetTransform().GetWorldScale();
    
    Vec2f offset = Vec2f(m_frameSize.x * 0.5f, m_frameSize.y * 0.5f);
    
    Transform transform;
    transform.Translate(globalPosition);
    transform.Translate(-offset);
    transform.Rotate(rotation);
    transform.Scale(scale);
    
    auto resourceManager = GET_RESMGR();
    auto shader = resourceManager->Get<Shader>("res/shaders/sprite.vert");
    if (!shader) return;

    Vec2i framePos = GetFramePosition(m_frame);

    float texWidth = static_cast<float>(GetTexture()->GetWidth());
    float texHeight = static_cast<float>(GetTexture()->GetHeight());
    
    const float u0 = framePos.x / texWidth;
    const float v0 = framePos.y / texHeight;
    const float u1 = (framePos.x + m_frameSize.x) / texWidth;
    const float v1 = (framePos.y + m_frameSize.y) / texHeight;
    
    renderer->SetTexture(GetTexture()->GetHandle());
    renderer->SetShader(shader->GetHandle());
    renderer->SetDepth(GetDepth());
    renderer->DrawQuad(
        globalPosition.x,
        globalPosition.y,
        static_cast<float>(m_frameSize.x),
        static_cast<float>(m_frameSize.y),
        u0, v0, u1, v1, Color::White()
    );
    
    Entity::OnRender(renderer);
}

int AnimatedSprite::GetFrame() const { return m_frame; }
void AnimatedSprite::SetFrame(int frame) {
    if (frame >= 0 && frame < m_frameCount) {
        m_frame = frame;
        m_time = 0;
    }
}

Vec2i AnimatedSprite::GetFrameSize() const { return m_frameSize; }
void AnimatedSprite::SetFrameSize(Vec2i size) {
    if (size.x > 0 && size.y > 0) {
        m_frameSize = size;
    }
}

Vec2i AnimatedSprite::GetFramePosition(int frame) const {
    if (frame < 0 || frame >= m_frameCount) return {0, 0};
    
    const int frameX = (frame % m_frameCount) * m_frameSize.x;
    const int frameY = (frame / m_frameCount) * m_frameSize.y;
    return {frameX, frameY};
}

bool AnimatedSprite::IsPlaying() const { return m_playing; }
void AnimatedSprite::Play(float frameDuration, bool loop) {
    m_frameDuration = frameDuration;
    m_playing = true;
    m_loop = loop;
}

void AnimatedSprite::Stop() {
    m_playing = false;
}
} // namespace Cleave