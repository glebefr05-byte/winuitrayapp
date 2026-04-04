#pragma once
#include <windows.h>
#include <string>

class Application {
private:
    static Application* instance;
    HANDLE mutexHandle;
    HWND mainWindow;
    bool isRunning;

    Application();
    ~Application();

public:
    static Application* GetInstance();
    bool Initialize();
    void Run();
    void Shutdown();
    bool IsAlreadyRunning();
    HWND GetMainWindow() const { return mainWindow; }

private:
    void CheckSingleInstance();
    void CreateMainWindow();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};