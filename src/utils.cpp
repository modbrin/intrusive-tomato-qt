#include "utils.h"


#ifdef Q_OS_WIN
#include <windows.h>
#include <WinUser.h>
#include "compositor_defines.h"
#endif // Q_OS_WIN

void enableWindowBackgroundBlur(QWidget* w)
{
    #ifdef Q_OS_WIN
    HWND hwnd = (HWND)w->winId();
    HMODULE hUser = GetModuleHandle(L"user32.dll");
    if (hUser)
    {
        pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute)
        {

            DWORD cx = 0xffff00ff;
            float cn = .5;
            DWORD alphaMask=0xff000000;
            DWORD nc = (cx|alphaMask)&((DWORD)(alphaMask*cn)|(~alphaMask));

            ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, nc, 0 };
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hwnd, &data);
        }
    }

    #endif // Q_OS_WIN
}

