#include "MainWindow.h"
#include "TrayApp_.h"
#include "TrayIcon.h"
#include "resource.h"
#include <string>

HWND MainWindow::hwnd = nullptr;
bool MainWindow::visible = false;

bool MainWindow::Create() {
    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"TrayAppMainWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));

    if (!RegisterClassExW(&wc)) {
        return false;
    }

    hwnd = CreateWindowExW(
        0,
        L"TrayAppMainWindow",
        L"Tray Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        nullptr, nullptr,
        wc.hInstance,
        nullptr
    );

    if (!hwnd) {
        return false;
    }

    CreateMenu();
    return true;
}

void MainWindow::CreateMenu() {
    HMENU hMenu = ::CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenuW(hFileMenu, MF_STRING, 1001, L"Exit");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

    SetMenu(hwnd, hMenu);
}

void MainWindow::Show() {
    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        visible = true;
    }
}

void MainWindow::Hide() {
    if (hwnd) {
        ShowWindow(hwnd, SW_HIDE);
        visible = false;
    }
}

void MainWindow::OnClose() {
    Hide(); // Закрываем окно, но не завершаем приложение
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static UINT uTaskbarRestart = RegisterWindowMessageW(L"TaskbarCreated");

    if (msg == WM_APP + 1 || msg == uTaskbarRestart) {
        return TrayIcon::TrayWndProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1001) { // Выход
            Application::GetInstance()->Shutdown();
        }
        break;

    case WM_DESTROY:
        Application::GetInstance()->Shutdown();
        break;

    case WM_CLOSE:
        Hide();
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}