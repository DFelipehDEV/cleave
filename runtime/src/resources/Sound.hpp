#pragma once
#include <string>

#include "Resource.hpp"

namespace Cleave {
class Sound : public Resource {
public:
    Sound() : m_data(nullptr) {};
    ~Sound() = default;

    std::string_view GetTypeName() const override { return "cleave::Sound"; }

    void* GetData() const;
    void SetData(void* data);
private:
    void* m_data;  // Pointer of backend specific structure
};

class SoundLoader : public ResourceLoader {
public:
    std::shared_ptr<Resource> Load(const std::string& path, ResourceManager* resourceManager) override;

    bool CanLoad(const std::string_view extension) const override {
        return extension == ".wav" || extension == ".mp3" ||
               extension == ".ogg" || extension == ".flac";
    }
};
}  // namespace Cleave