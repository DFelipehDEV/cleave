#ifdef _WIN32

#include "platform/FileDialog.hpp"
#include <windows.h>
#include <commdlg.h>
#include <tchar.h>

std::string FileDialog::OpenFile(const char* filter, const char* initialDir) {
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[260] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = initialDir;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return std::string(ofn.lpstrFile);
    }

    return "";
}

std::string FileDialog::SaveFile(const char *filter, const char *initialDir) {
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[260] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = initialDir;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn) == TRUE) {
        return std::string(ofn.lpstrFile);
    }

    return "";
}

#endif
