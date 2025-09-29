#pragma once

#include "entities/Sprite.hpp"

namespace Cleave {
class AnimatedSprite : public Sprite {
public:
    AnimatedSprite() = default;
    AnimatedSprite(Transform transform,
                   std::shared_ptr<Texture> texture,
                   Vec2i frameSize, int frameCount)
        : Sprite(transform, texture),
          m_frameSize(frameSize),
          m_frameCount(frameCount) {}

    void OnTick(float deltaTime) override;
    void OnRender(Renderer* renderer) override;

    static const char* GetTypeName() { return "cleave::AnimatedSprite"; }

    const PropertyMap GetProperties() const override;
    void SetProperty(std::string_view name, const std::string& value) override;

    static Entity* Create();

    bool IsPlaying() const;
    void Play(float frameDuration, bool loop = true);
    void Stop();

    int GetFrame() const;
    void SetFrame(int frame);

    Vec2i GetFrameSize() const;
    void SetFrameSize(Vec2i size);

    Vec2i GetFramePosition(int frame) const;
private:
    Vec2i m_frameSize = {32, 32};

    int m_frameCount = 1;
    int m_frame = 0;
    float m_time = 0.0f;
    float m_frameDuration = 0.1f;
    bool m_playing = false;
    bool m_loop = true;
};
}  // namespace Cleave