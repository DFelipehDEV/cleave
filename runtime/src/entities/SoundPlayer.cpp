#include "entities/SoundPlayer.hpp"
#include "services/Services.hpp"
#include "Log.hpp"

namespace Cleave {
const Entity::PropertyMap SoundPlayer::GetProperties() const {
    auto properties = Entity::GetProperties();
    properties["type"] = {GetTypeName(), Property::Types::Hidden};
    std::string soundPath;
    if (m_sound)
        soundPath = m_sound->GetPath();
    else
        soundPath = "";
    properties["sound"] = {soundPath, Property::Types::FilePath};
    properties["playing"] = {std::to_string(IsPlaying()), Property::Types::Bool};
    properties["loop"] = {std::to_string(m_loop), Property::Types::Bool};
    properties["volume"] = {std::to_string(m_volume), Property::Types::Float};
    return properties;
}

void SoundPlayer::SetProperty(std::string_view name, const std::string& value) {
    if (name == "sound") {
        auto sound = GET_RESMGR()->Get<Sound>(value);
        if (sound) {
            m_sound = sound;
        } else {
            LOG_WARN("Sound in '" << value << "' doesn't exist");
        }
    } else if (name == "playing") {
        m_playing = std::stoi(value);
        if (m_playing) 
            Play(); 
        else 
            Stop();
    } else if (name == "loop") {
        SetLoop(std::stoi(value));
    } else if (name == "volume") {
        SetVolume(std::stof(value));
    } else {
        Entity::SetProperty(name, value);
    }
}

Entity* SoundPlayer::Create() { return new SoundPlayer(); }

bool SoundPlayer::IsPlaying() const { return m_playing; }
void SoundPlayer::Play() {
    m_playing = true;
    m_soundHandle = GET_AUDIOMGR()->PlaySound(m_sound);
}

void SoundPlayer::Stop() {
    m_playing = false;
    GET_AUDIOMGR()->StopSound(m_soundHandle);
}

float SoundPlayer::GetVolume() const { return m_volume; }
void SoundPlayer::SetVolume(float volume) {
    m_volume = volume;
    GET_AUDIOMGR()->SetSoundVolume(m_soundHandle, m_volume);
}

bool SoundPlayer::IsLooping() const { return m_loop; }
void SoundPlayer::SetLoop(bool loop) {
    m_loop = loop;
    GET_AUDIOMGR()->SetSoundLoop(m_soundHandle, m_loop);
}
} // namespace Cleave