#include "TrayApp_.h"
#include "TrayIcon.h"
#include "MainWindow.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    // Проверка на повторный запуск
    Application* app = Application::GetInstance();

    if (app->IsAlreadyRunning()) {
        MessageBoxW(nullptr, L"App already started", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Инициализация приложения
    if (!app->Initialize()) {
        MessageBoxW(nullptr, L"Error initialising!", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Главное окно изначально скрыто
    MainWindow::Hide();

    // Запуск основного цикла
    app->Run();

    return 0;
}