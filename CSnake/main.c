#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>

// Need to define UNICODE to avoid LPCSTR being used instead of wchat_t
#ifndef UNICODE
#define UNICODE
#endif 

// Initializes the window procedure signature
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnResize(HWND hwnd, UINT flag, int width, int height);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE pInstance, PWSTR pCmdLine, int nCmdShow) {
    /*
    Initialize the window class
    */

    // wchar_t is used instead of char as the Windows API requires it
    // wchar_t shouldn't be used anywhere else besides the Windows API as it uses more multiple bytes (long char)
    const wchar_t CLASS_NAME[] = L"Main Window Class";
    WNDCLASS wc = { 0 };

    // lpfnWndProc is a pointer to the window procedure function, defines ("most of") the behavior of the window
    wc.lpfnWndProc = WindowProc;
    // hInstance is the handle to the application instance
    wc.hInstance = hInstance;
    // lpszClassName is the string identifier of the window class
    wc.lpszClassName = CLASS_NAME;

    // Registers the window class with the operating system
    RegisterClass(&wc);

    /*
    create the window
    */

    HWND hwnd = CreateWindowEx(
        0,                      // Optional window styles, ex. transparent window
        CLASS_NAME,             // Window class, uses the name to access which was previously registered
        L"CSnake",              // Window text
        WS_OVERLAPPEDWINDOW,    // Window style, multiple flags in one, creates the title bar, min/max buttons, etc.

        // Size and Position (xPos, yPos, width, height)
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

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

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

// WindowProc(windowHandle, message, additionalParameter, additionalParameter)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // Stops the process when the window is closed
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Runs on window resize
        case WM_SIZE:
            int width = LOWORD(lParam);  // Macro to get the low-order word.
            int height = HIWORD(lParam); // Macro to get the high-order word.
            OnResize(hwnd, (UINT)wParam, width, height);
            return 0;

        // Fills ("paints") the window
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hdc, &ps);
            return 0;

        // Fix the mouse cursor not changing when hovered over the window
        // ex. stuck on resize horizontal cursor after moving into the window from the right
        //TODO: make sure this is the efficient way to do this (mousehover doesn't work)
        case WM_MOUSEMOVE:
            HCURSOR defaultCur = LoadCursor(NULL, IDC_ARROW);
            SetCursor(defaultCur);
            return 0;

    }
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnResize(HWND hwnd, UINT flag, int width, int height) {
    
}