#include "utils.h"

#include <QMessageBox>

#ifdef Q_OS_WIN
#include <windows.h>
#include <WinUser.h>
#include <QSettings>
#include <QCoreApplication>
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

static QString launchKey("IntrusiveTomatoQtAutostartKey");
static QString autostartKeyLocation("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

// check and return if autostart is enabled for the app
// also update the path of startup - needed if app is moved, so restarting it will fix path
bool updateAutostartPath() {
    if (isAutostartEnabled()) {
        return setAutostartEnabled(true);
    } else {
        return false;
    }
}

// returns success status
bool setAutostartEnabled(bool isEnabled)
{
    #ifdef Q_OS_WIN
    QSettings bootUpSettings(
        autostartKeyLocation,
        QSettings::NativeFormat
    );
    QString apostroph = "\"";
    QString app_path = apostroph + QCoreApplication::applicationFilePath() + apostroph;
    app_path.replace("/", "\\");
    if (isEnabled) {
        bootUpSettings.setValue(launchKey, app_path);
    } else {
        bootUpSettings.remove(launchKey);
    }
    return isEnabled == isAutostartEnabled();
    #else
    return false;
    #endif
}

bool isAutostartEnabled() {
    #ifdef Q_OS_WIN
    QSettings settings (
        autostartKeyLocation,
        QSettings::NativeFormat
    );
    return settings.childKeys().contains(launchKey, Qt::CaseInsensitive);
    #else
    return false;
    #endif // Q_OS_WIN
}

void showMessage(const QString& msg) {
    QMessageBox mb;
    mb.setText(msg);
    mb.exec();
}
