#include "TrayIcon.h"
#include "MainWindow.h"
#include "TrayApp_.h"
#include "resource.h"

TrayIcon* TrayIcon::instance = nullptr;

TrayIcon::TrayIcon()
    : parentWindow(nullptr)
    , initialized(false) {
    ZeroMemory(&nid, sizeof(nid));
}

TrayIcon::~TrayIcon() {
    Cleanup();
}

TrayIcon* TrayIcon::GetInstance() {
    if (!instance) {
        instance = new TrayIcon();
    }
    return instance;
}

bool TrayIcon::Initialize(HWND parent) {
    parentWindow = parent;

    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = parentWindow;
    nid.uID = 100;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_APP + 1;
    nid.hIcon = (HICON)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    wcscpy_s(nid.szTip, L"Tray Application");

    initialized = (Shell_NotifyIconW(NIM_ADD, &nid) == TRUE);

    if (initialized) {
        // Обработка пересоздания панели задач
        RegisterWindowMessage(L"TaskbarCreated");
    }

    return initialized;
}

void TrayIcon::Cleanup() {
    if (initialized) {
        Shell_NotifyIconW(NIM_DELETE, &nid);
        initialized = false;
    }
}

void TrayIcon::ShowContextMenu() {
    HMENU hMenu = CreatePopupMenu();

    AppendMenuW(hMenu, MF_STRING, 1, L"Open");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, 2, L"Exit");

    POINT pt;
    GetCursorPos(&pt);

    SetForegroundWindow(parentWindow);

    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, parentWindow, nullptr);

    switch (cmd) {
    case 1: // Открыть
        MainWindow::Show();
        break;
    case 2: // Выход
        Application::GetInstance()->Shutdown();
        break;
    }

    DestroyMenu(hMenu);
    PostMessage(parentWindow, WM_NULL, 0, 0);
}

void TrayIcon::UpdateAfterTaskbarRestart() {
    if (initialized) {
        Shell_NotifyIconW(NIM_DELETE, &nid);
        Shell_NotifyIconW(NIM_ADD, &nid);
    }
}

LRESULT CALLBACK TrayIcon::TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_APP + 1) {
        switch (lParam) {
        case WM_LBUTTONDOWN:
            MainWindow::Show();
            break;
        case WM_RBUTTONDOWN:
            GetInstance()->ShowContextMenu();
            break;
        }
    }
    static UINT s_uTaskbarRestart = RegisterWindowMessage(L"TaskbarCreated");
    if (msg == s_uTaskbarRestart) {
        GetInstance()->UpdateAfterTaskbarRestart();
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}