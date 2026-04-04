#pragma once
#include <windows.h>

class MainWindow {
private:
    static HWND hwnd;
    static bool visible;

public:
    static bool Create();
    static void Show();
    static void Hide();
    static HWND GetHandle() { return hwnd; }
    static void OnClose();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void CreateMenu();
};