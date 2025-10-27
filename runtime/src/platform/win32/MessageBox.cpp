#include "platform/MessageBox.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef MessageBox

namespace Cleave {
int MessageBox::Show(const char* title, const char* content, uint32_t flags) {
    wchar_t* wTitle = new wchar_t[4096];
    wchar_t* wContent = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, title, -1, wTitle, 4096);
    MultiByteToWideChar(CP_ACP, 0, content, -1, wContent, 4096);
    MessageBoxW(NULL, wContent, wTitle, flags);
    delete[] wTitle;
    delete[] wContent;

    return 0;
}
}  // namespace Cleave