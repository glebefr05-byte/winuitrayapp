#include "TrayApp_.h"
#include "MainWindow.h"
#include "TrayIcon.h"
#include <memory>

Application* Application::instance = nullptr;

Application::Application()
    : mutexHandle(nullptr)
    , mainWindow(nullptr)
    , isRunning(true) {
}

Application::~Application() {
    if (mutexHandle != nullptr && mutexHandle != INVALID_HANDLE_VALUE) {
        ReleaseMutex(mutexHandle);
        CloseHandle(mutexHandle);
    }
}

Application* Application::GetInstance() {
    if (!instance) {
        instance = new Application();
    }
    return instance;
}

bool Application::IsAlreadyRunning() {
    CheckSingleInstance();
    return mutexHandle == nullptr;
}

void Application::CheckSingleInstance() {
    mutexHandle = CreateMutexW(NULL, TRUE, L"Global\\{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}");

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(mutexHandle);
        mutexHandle = nullptr;
    }
}

void Application::CreateMainWindow() {
    MainWindow::Create();
    mainWindow = MainWindow::GetHandle();
}

bool Application::Initialize() {

    if (!mutexHandle) {
        return false;
    }

    CreateMainWindow();

    // Инициализация трея
    if (!TrayIcon::GetInstance()->Initialize(mainWindow)) {
        return false;
    }

    return true;
}

void Application::Run() {
    MSG msg;

    while (isRunning && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Application::Shutdown() {
    TrayIcon::GetInstance()->Cleanup();
    isRunning = false;
    PostQuitMessage(0);
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}