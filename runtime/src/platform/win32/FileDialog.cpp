#ifdef _WIN32

#include "platform/FileDialog.hpp"

#include <atlbase.h>
#include <commdlg.h>
#include <shobjidl.h>
#include <tchar.h>
#include <windows.h>

namespace Cleave {
std::string FileDialog::SaveFile(const char* filter, const char* initialDir) {
    OPENFILENAME ofn = {0};
    TCHAR szFile[MAX_PATH] = {0};

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

std::string FileDialog::OpenFile(const char* filter, const char* initialDir) {
    OPENFILENAME ofn = {0};
    TCHAR szFile[MAX_PATH] = {0};

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

std::string FileDialog::OpenFolder(const char* initialDir) {
    CoInitialize(nullptr);

    CComPtr<IFileOpenDialog> pFolderDlg;
    pFolderDlg.CoCreateInstance(CLSID_FileOpenDialog);

    FILEOPENDIALOGOPTIONS opt{};
    pFolderDlg->GetOptions(&opt);
    pFolderDlg->SetOptions(opt | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST |
                           FOS_FORCEFILESYSTEM);

    if (SUCCEEDED(pFolderDlg->Show(nullptr))) {
        CComPtr<IShellItem> pSelectedItem;
        pFolderDlg->GetResult(&pSelectedItem);

        CComHeapPtr<wchar_t> pPath;
        pSelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);

        char path[MAX_PATH];
        size_t i;
        wcstombs_s(&i, path, MAX_PATH, pPath.m_pData, MAX_PATH);

        CoUninitialize();
        return std::string(path);
    }

    CoUninitialize();
    return "";
}
}  // namespace Cleave
#endif