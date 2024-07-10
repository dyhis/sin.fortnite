#pragma once
#include <windows.h>
#include "../xor.h"
#include <utils/encryption/callstack.h>

class key_state_handler {
public:
    void load() {
        hide;
        HMODULE user32 = hide_call(LoadLibraryA)(enc("user32.dll"));
        if (!user32) {
            return;
        }

        void* GetAsyncKeyStateProc = (void*)GetProcAddress(user32, enc("GetAsyncKeyState"));
        if (!GetAsyncKeyStateProc) {
            hide_call(FreeLibrary)(user32);
            return;
        }

        *(void**)&_GetAsyncKeyState = GetAsyncKeyStateProc;
        hide_call(FreeLibrary)(user32);
    }

    SHORT get(int vKey) {
        if (_GetAsyncKeyState != nullptr) {
            return _GetAsyncKeyState(vKey);
        }
        return 0;
    }

private:
    SHORT(*_GetAsyncKeyState)(int vKey);
};
inline std::unique_ptr<key_state_handler> key_handler = std::make_unique<key_state_handler>();