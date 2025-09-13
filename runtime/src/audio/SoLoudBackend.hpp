#pragma once
#include "audio/AudioManager.hpp"
#include <memory>
#include <unordered_map>

#include "thirdparty/soloud/include/soloud.h"
#include "thirdparty/soloud/include/soloud_wav.h"

namespace Cleave {
class SoLoudBackend : public AudioBackend {
public:
    SoLoudBackend::SoLoudBackend() : m_engine(new SoLoud::Soloud()) {}
    SoLoudBackend::~SoLoudBackend() {
        Shutdown();
    }
    
    bool Init() override;
    void Shutdown() override;
    
    bool LoadSound(std::shared_ptr<Sound> sound) override;
    SoundHandle PlaySound(std::shared_ptr<Sound> sound, float volume) override;
    void StopSound(SoundHandle handle) override;

    void PlayMusic(std::shared_ptr<Sound> sound, float volume) override;
    
    void StopAllSounds() override;
    void StopMusic() override;
    
    void SetSoundVolume(float volume) override;
    void SetSoundVolume(SoundHandle handle, float volume) override;
    void SetMusicVolume(float volume) override;

    void SetSoundLoop(SoundHandle handle, bool loop) override;
private:
    SoLoud::Soloud* m_engine;
    std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> m_sounds;
    float m_soundVolume = 1.0f;
    float m_musicVolume = 1.0f;
    int m_musicHandle = 0;
};
}  // namespace Cleave