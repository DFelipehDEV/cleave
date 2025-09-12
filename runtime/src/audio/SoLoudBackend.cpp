#include "audio/SoLoudBackend.hpp"

namespace Cleave {
bool SoLoudBackend::Init() {
    return m_engine->init() == 0;
}

void SoLoudBackend::Shutdown() {
    m_engine->deinit();
    m_sounds.clear();
}

bool SoLoudBackend::LoadSound(std::shared_ptr<Sound> sound) {
    // Check if already loaded
    if (sound->GetData() != nullptr) {
        return true;
    }
    
    auto* wav = new SoLoud::Wav();
    if (wav->load(sound->GetPath().c_str()) != 0) {
        delete wav;
        return false;
    }
    
    sound->SetData(wav);
    return true;
}

void SoLoudBackend::PlaySound(std::shared_ptr<Sound> sound, float volume) {
    if (!sound->GetData()) {
        LoadSound(sound);
    }
    
    if (auto* wav = static_cast<SoLoud::Wav*>(sound->GetData())) {
        m_engine->play(*wav, volume * m_soundVolume);
    }
}

void SoLoudBackend::PlayMusic(std::shared_ptr<Sound> sound, float volume) {
    if (!sound->GetData()) {
        LoadSound(sound);
    }

    if (m_musicHandle != 0) {
        m_engine->stop(m_musicHandle);
    }
    
    if (auto* wav = static_cast<SoLoud::Wav*>(sound->GetData())) {
        m_musicHandle = m_engine->play(*wav, volume * m_musicVolume);
        m_engine->setLooping(m_musicHandle, true);
    }
}

void SoLoudBackend::StopAllSounds() {
    m_engine->stopAll();
}

void SoLoudBackend::StopMusic() {
    if (m_musicHandle != 0) {
        m_engine->stop(m_musicHandle);
        m_musicHandle = 0;
    }
}

void SoLoudBackend::SetSoundVolume(float volume) { m_soundVolume = volume; }
void SoLoudBackend::SetMusicVolume(float volume) {
    m_musicVolume = volume;
    if (m_musicHandle != 0) {
        m_engine->setVolume(m_musicHandle, volume);
    }
}
} // namespace Cleave