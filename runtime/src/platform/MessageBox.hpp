#pragma once
#include <stdint.h>

class MessageBox {
public:
    enum ButtonFlags {
        Ok = 0,
        OkCancel,
        YesNo = 4,
    };

    enum IconFlags {
        Error = 16,
        Question = 32,
        Warning = 48,
        Information = 64,
    };

    static int Show(const char* title, const char* content,
                    uint32_t flags = ButtonFlags::Ok | IconFlags::Information);
};