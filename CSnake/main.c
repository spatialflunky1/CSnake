#include <stdio.h>
#include <Windows.h>

// Need to define UNICODE to avoid LPCSTR being used instead of wchat_t
#ifndef UNICODE
#define UNICODE
#endif 

// Initializes the window procedure signature
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE pInstance, PWSTR pCmdLine, int nCmdShow) {
    /*
    Initialize the window class
    */

    // wchar_t is used instead of char as the Windows API requires it
    // wchar_t shouldn't be used anywhere else besides the Windows API as it uses more multiple bytes (long char)
    const wchar_t CLASS_NAME[] = L"Main Window Class";
    WNDCLASSW wc;

    // lpfnWndProc is a pointer to the window procedure function, defines ("most of") the behavior of the window
    wc.lpfnWndProc = WindowProc;
    // hInstance is the handle to the application instance
    wc.hInstance = hInstance;
    // lpszClassName is the string identifier of the window class
    wc.lpszClassName = CLASS_NAME;

    // Registers the window class with the operating system
    RegisterClassW(&wc);

    /*
    create the window
    */

    HWND hwnd = CreateWindowExW(
        0,                      // Optional window styles, ex. transparent window
        CLASS_NAME,             // Window class, uses the name to access which was previously registered
        L"CSnake",              // Window text
        WS_OVERLAPPEDWINDOW,    // Window style, multiple flags in one, creates the title bar, min/max buttons, etc.

        // Size and Position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,           // Parent window, for subwindows within the main window
        NULL,           // Menu
        hInstance,      // Intance handle
        NULL            // Additional application data
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    /*
    loop the program
    */
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_SIZE:

    }
}