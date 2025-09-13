#pragma once
#include <vector>
#include <string>
#include <memory>

#include "resources/ResourceManager.hpp"
#include "resources/Sound.hpp"

namespace Cleave {
typedef uint32_t SoundHandle;

class AudioBackend {
public:
    virtual ~AudioBackend() = default;
    
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    
    virtual bool LoadSound(std::shared_ptr<Sound> sound) = 0;
    virtual SoundHandle PlaySound(std::shared_ptr<Sound> sound, float volume = 1.0f) = 0;
    virtual void StopSound(SoundHandle handle) = 0;

    virtual void PlayMusic(std::shared_ptr<Sound> sound, float volume = 1.0f) = 0;
    
    virtual void StopAllSounds() = 0;
    virtual void StopMusic() = 0;
    
    virtual void SetSoundVolume(float volume) = 0;
    virtual void SetSoundVolume(SoundHandle handle, float volume) = 0;
    virtual void SetMusicVolume(float volume) = 0;

    virtual void SetSoundLoop(SoundHandle handle, bool loop) = 0;
};

class AudioManager {
public:
    AudioManager(ResourceManager* resourceManager, std::unique_ptr<AudioBackend> backend) : m_resourceManager(resourceManager), m_backend(std::move(backend)) {
        m_backend->Init();
    };
    ~AudioManager() = default;

    SoundHandle PlaySound(std::shared_ptr<Sound> sound);
    void StopSound(SoundHandle handle);
    void PlayMusic(std::shared_ptr<Sound> music);

    void StopAllSounds();

    float GetMusicVolume();
    void SetMusicVolume(float volume);

    float GetSoundVolume();
    void SetSoundVolume(float volume);
    void SetSoundVolume(SoundHandle handle, float volume);

    void SetSoundLoop(SoundHandle handle, bool loop);
private:
    std::unique_ptr<AudioBackend> m_backend;
    ResourceManager* m_resourceManager;
    float m_musicVolume = 1.0f;
    float m_soundVolume = 1.0f;
};
}  // namespace Cleave