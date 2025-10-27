#pragma once
#include "entities/Entity.hpp"
#include "resources/Sound.hpp"
#include "services/AudioManager.hpp"

namespace Cleave {
class SoundPlayer : public Entity {
public:
    SoundPlayer() = default;
    SoundPlayer(Transform transform) : Entity(transform) {}
    
    static const char* GetTypeName() { return "cleave::SoundPlayer"; }

    const PropertyMap GetProperties() const override;
    void SetProperty(const std::string_view name, const std::string& value) override;

    static Entity* Create();

    bool IsPlaying() const;
    void Play();
    void Stop();

    float GetVolume() const;
    void SetVolume(float volume);

    bool IsLooping() const;
    void SetLoop(bool loop);
private:
    std::shared_ptr<Sound> m_sound;
    SoundHandle m_soundHandle = 0;
    bool m_playing = false;
    bool m_loop = false;
    float m_volume = 1.0f;
};
} // namespace Cleave