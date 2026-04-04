#pragma once
#include <windows.h>
#include <shellapi.h>

class TrayIcon {
private:
    static TrayIcon* instance;
    NOTIFYICONDATAW nid;
    HWND parentWindow;
    bool initialized;

    TrayIcon();
    ~TrayIcon();

public:
    static TrayIcon* GetInstance();
    bool Initialize(HWND parent);
    void Cleanup();
    void ShowContextMenu();
    void UpdateAfterTaskbarRestart();
    static LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    void CreateContextMenu();
    
};