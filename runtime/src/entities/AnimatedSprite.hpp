#pragma once

#include "entities/Sprite.hpp"

namespace Cleave {
class AnimatedSprite : public Sprite {
public:
    AnimatedSprite() = default;
    AnimatedSprite(Transform transform,
                   std::shared_ptr<Texture> texture,
                   int frameWidth, int frameHeight)
        : Sprite(transform, texture),
          m_frameWidth(frameWidth),
          m_frameHeight(frameHeight),
          m_framesX(texture ? texture->GetWidth() / frameWidth : 0),
          m_framesY(texture ? texture->GetHeight() / frameHeight : 0) {}

    void Init(const std::unordered_map<std::string, Property> properties) override;
    void OnTick(float deltaTime) override;
    void OnRender(Renderer* renderer) override;

    static const char* GetTypeName() { return "cleave::AnimatedSprite"; }

    const std::unordered_map<std::string, Property> GetProperties() const override;

    static Entity* Create();

    void Play(float fps, bool loop = true);
    void Stop();
    void SetFrame(int frame);

    bool IsPlaying() const { return m_playing; }
    int GetCurrentFrame() const { return m_currentFrame; }

private:
    int m_frameWidth = 32;
    int m_frameHeight = 32;
    int m_framesX;
    int m_framesY;

    int m_currentFrame = 0;
    float m_time = 0.0f;
    float m_frameDuration = 0.1f;
    bool m_playing = false;
    bool m_loop = true;
};
}  // namespace Cleave