#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include <WinUser.h>
#include <wingdi.h>
#include "main.h"
#include "utils.h"
#include "game.h"

// global variables
int score = 0;
int direction = 0;
int increase = 0;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE pInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
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

    // Creates the menubar
    HMENU menubar = CreateMenu();
    HMENU fileMenu = CreateMenu();
    HMENU editMenu = CreateMenu();
    HMENU helpMenu = CreateMenu();

    AppendMenu(menubar, MF_POPUP, (UINT_PTR)fileMenu, L"&File");
    AppendMenu(fileMenu, MF_STRING, ID_QUIT, L"&Quit");

    AppendMenu(menubar, MF_POPUP, (UINT_PTR)editMenu, L"&Edit");
    AppendMenu(editMenu, MF_STRING, ID_SETTINGS, L"&Settings");

    AppendMenu(menubar, MF_POPUP, (UINT_PTR)helpMenu, L"&Help");
    AppendMenu(helpMenu, MF_STRING, ID_ABOUT, L"&About");


    HWND hwnd = CreateWindowEx(
        0,                      // Optional window styles, ex. transparent window
        CLASS_NAME,             // Window class, uses the name to access which was previously registered
        L"CSnake",              // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,    // Window style, multiple flags in one, creates the title bar, min/max buttons, etc.

        // Size and Position (xPos, yPos, width, height)
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,

        NULL,           // Parent window, for subwindows within the main window
        menubar,        // Menu
        hInstance,      // Intance handle
        NULL            // Additional application data
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    /*
    loop the program
    */

    DWORD threadID = 0;
    HANDLE gameThread = CreateThread(NULL, 0, gameLoop, hwnd, 0, &threadID);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

DWORD WINAPI gameLoop(HWND hwnd) {
    struct snake snake1 = { NULL, NULL, NULL };
    int** temp = create2dArrayofSize(3);
    temp[0][0] = 309; temp[0][1] = 207;
    temp[1][0] = 295; temp[1][1] = 207;
    snake1.curr = temp;

    HDC hdc = GetDC(hwnd);
    HBRUSH blackBrush = CreateSolidBrush(RGB(0,0,0));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN blackPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
    HPEN whitePen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    while (TRUE) {
        snakeMove(hdc, &snake1, whiteBrush, blackBrush, whitePen, blackPen, direction, &increase);
    }
    DeleteObject(blackBrush);
    DeleteObject(whiteBrush);
    DeleteObject(blackPen);
    DeleteObject(whitePen);
    ReleaseDC(hwnd, hdc);
}

// WindowProc(windowHandle, message, additionalParameter, additionalParameter)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    switch (uMsg) {
        // Stops the process when the window is closed
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Fills ("paints") the window
        case WM_PAINT:
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            printString(hdc, 5, 5, L"Score:");
            printNum(hdc, 53, 5, score);
            EndPaint(hwnd, &ps);
            return 0;

        // Fix the mouse cursor not changing when hovered over the window
        // ex. stuck on resize horizontal cursor after moving into the window from the right
        //TODO: make sure this is the efficient way to do this (mousehover doesn't work)
        case WM_MOUSEMOVE:
            HCURSOR defaultCur = LoadCursor(NULL, IDC_ARROW);
            SetCursor(defaultCur);
            return 0;

        // Recieves the input for buttons being pressed
        case WM_COMMAND:
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case ID_QUIT:
                    PostQuitMessage(0);
                    return 0;

                case ID_ABOUT:
                    MessageBox(hwnd, L"CSnake 2023 AP CSA Final Project", L"About CSnake", MB_OK | MB_ICONINFORMATION);
                    return 0;
            }
            return 0;

        case WM_KEYDOWN:
            // Left
            if ((wParam == 37 || wParam == 65) && direction != 4) direction = 3;
            // Up
            else if ((wParam == 38 || wParam == 87) && direction != 2) direction = 1;
            // Right
            else if ((wParam == 39 || wParam == 68) && direction != 3) direction = 4;
            // Down
            else if ((wParam == 40 || wParam == 83) && direction != 1) direction = 2;
            else if (wParam == 32) increase=1;

    }
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}