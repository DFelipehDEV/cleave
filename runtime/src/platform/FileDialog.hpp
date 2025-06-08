#pragma once
#include <string>

class FileDialog {
public:
    static std::string SaveFile(
        const char* filter,
        const char* initialDir = ""
    );
    static std::string OpenFile(
        const char* filter,
        const char* initialDir = ""
    );
    static std::string OpenFolder(
        const char* initialDir = ""
    );
};