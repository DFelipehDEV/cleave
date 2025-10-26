#pragma once
#include <filesystem>
#include <string>

namespace Cleave {
namespace Editor {

class EditorContext;
class FileExplorer {
public:
    FileExplorer(const std::filesystem::path& directory, EditorContext* editorContext)
        : m_directory(directory), m_editorContext(editorContext) {}
    ~FileExplorer() = default;

    void ShowDirectory(const std::filesystem::path& dir);

    void OnRender();

    const std::filesystem::path& GetDirectory() const;
    void SetDirectory(const std::filesystem::path& directory);

private:
    EditorContext* m_editorContext;
    std::filesystem::path m_directory;
    std::filesystem::path m_selectedDirectory;
    bool m_isRenaming = false;
    std::string m_renameBuffer;
};
}  // namespace Editor
}  // namespace Cleave