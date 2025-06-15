#pragma once
#include <filesystem>
#include <string>

namespace Cleave {
namespace Editor {
class FileExplorer {
public:
    FileExplorer(const std::filesystem::path& directory)
        : m_directory(directory) {}
    ~FileExplorer() = default;

    void ShowDirectory(std::filesystem::path dir);

    void OnRender();

    std::filesystem::path GetDirectory() const;
    void SetDirectory(std::filesystem::path directory);

private:
    std::filesystem::path m_directory;
    std::filesystem::path m_selectedDirectory;
};
}  // namespace Editor
}  // namespace Cleave