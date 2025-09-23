#include "resources/Sound.hpp"

#include <algorithm>
namespace Cleave {
    void* Sound::GetData() const { return m_data; }
    void Sound::SetData(void* data) { m_data = data; }

    std::shared_ptr<Resource> SoundLoader::Load(const std::string& path, ResourceManager* resourceManager) {
        auto sound = std::make_shared<Sound>();
        sound->SetPath(path);
        return sound;
    }
} // namespace Cleave