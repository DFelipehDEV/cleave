#pragma once
#include <string>
#include <filesystem>

class FileExplorer {
public:
    FileExplorer(const std::filesystem::path& directory) : m_directory(directory) {}
    ~FileExplorer() = default;

    void ShowDirectory(std::filesystem::path dir);

    void OnRender();

private:
    std::filesystem::path m_directory;
    std::filesystem::path m_selectedDirectory;
};