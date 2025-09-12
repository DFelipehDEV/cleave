#include "audio/AudioManager.hpp"

namespace Cleave {
    void AudioManager::PlaySound(std::shared_ptr<Sound> sound) {
        if (m_backend) {
            m_backend->PlaySound(sound, m_soundVolume);
        }
    }

    void AudioManager::PlayMusic(std::shared_ptr<Sound> music) {
        if (m_backend) {
            m_backend->PlayMusic(music, m_soundVolume);
        }
    }

    void AudioManager::StopAllSounds() {
        if (m_backend) {
            m_backend->StopAllSounds();
        }
    }

    void AudioManager::SetSoundVolume(float volume) {
        m_soundVolume = volume;
        if (m_backend) {
            m_backend->SetSoundVolume(volume);
        }
    }

    void AudioManager::SetMusicVolume(float volume) {
        m_musicVolume = volume;
        if (m_backend) {
            m_backend->SetMusicVolume(volume);
        }
    }
} // namespace Cleave