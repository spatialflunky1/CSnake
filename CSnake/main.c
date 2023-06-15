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
int direction = 0;
HBRUSH snakeBrush;
HBRUSH backgroundBrush;
HBRUSH appleBrush;
HPEN snakePen;
HPEN backgroundPen;
HPEN applePen;
// wchar_t is used instead of char as the Windows API requires it
// wchar_t shouldn't be used anywhere else besides the Windows API as it uses more multiple bytes (long char)
const wchar_t CLASS_NAME[] = L"Main Window Class";
const wchar_t CLASS_NAME_SETTINGS[] = L"Settings Window Class";

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE pInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
    /*
    Initialize the window class
    */

    // Main window class and window creation
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc; // pointer to the window procedure function
    wc.hInstance = hInstance; // handle to the application instance
    wc.lpszClassName = CLASS_NAME; // string identifier of the window class
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Registers the window class with the operating system
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Main window class creation failed", L"Fatal Error!", MB_ICONERROR);
        return 0;
    }

    // Settings window class and window creation
    WNDCLASS wc_s = { 0 };
    wc_s.lpfnWndProc = WindowProcSettings;
    wc_s.hInstance = hInstance;
    wc_s.lpszClassName = CLASS_NAME_SETTINGS;
    wc_s.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc_s)) {
        MessageBox(NULL, L"Settings window class creation failed", L"Fatal Error!", MB_ICONERROR);
        return 0;
    }

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

    if (hwnd == NULL) {
        MessageBox(NULL, L"Main window creation failed", L"Fatal Error!", MB_ICONERROR);
        return 0;
    }

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
    struct snake snake1;
    int** temp = create2dArrayofSize(1);
    temp[0][0] = 330; temp[0][1] = 210;
    snake1.curr = temp;

    // Initialize rand for apple position
    srand(time(NULL));
    setRandApple(&snake1);

    HDC hdc = GetDC(hwnd);
    /*
    Snake: Black
    Background: White
    Apple: Red
    */
    snakeBrush = CreateSolidBrush(RGB(0,0,0));
    backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
    appleBrush = CreateSolidBrush(RGB(255, 0, 0));
    snakePen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
    backgroundPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    applePen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));

    while (TRUE) {
        if (snakeMove(hdc, &snake1, backgroundBrush, snakeBrush, appleBrush, backgroundPen, snakePen, applePen, direction) == -1) break;
    }

    printString(hdc, 270, 200, L"Game Over!");

    DeleteObject(snakeBrush);
    DeleteObject(backgroundBrush);
    DeleteObject(appleBrush);
    DeleteObject(snakePen);
    DeleteObject(backgroundPen);
    DeleteObject(applePen);
    ReleaseDC(hwnd, hdc);
}

DWORD WINAPI settingsThread(HWND settingsHwnd) {
    MSG msg = { 0 };
    while (GetMessage(&msg, settingsHwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// WindowProc(windowHandle, message, additionalParameter, additionalParameter)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // Stops the process when the window is closed
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Fills ("paints") the window
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            Rectangle(hdc, 0, 0, 84, 24);
            printString(hdc, 5, 5, L"Score:");
            EndPaint(hwnd, &ps);
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

                case ID_SETTINGS:
                    HINSTANCE hInstance = GetModuleHandle(NULL);
                    HWND settingsHwnd = CreateWindowEx(
                        0,
                        CLASS_NAME_SETTINGS,
                        L"CSnake Settings",
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                        // Size and Position (xPos, yPos, width, height)
                        CW_USEDEFAULT, CW_USEDEFAULT, 400, 320,
                        hwnd,
                        NULL,
                        hInstance,
                        NULL
                    );

                    if (settingsHwnd == NULL) {
                        MessageBox(NULL, L"Settings window creation failed", L"Fatal Error!", MB_ICONERROR);
                        return 0;
                    }

                    ShowWindow(settingsHwnd, SW_SHOW);
                    DWORD threadID2 = 1;
                    HANDLE sThread = CreateThread(NULL, 0, settingsThread, settingsHwnd, 0, &threadID2);
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

    }
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// WindowProcSettings(windowHandle, message, additionalParameter, additionalParameter)
LRESULT CALLBACK WindowProcSettings(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    }
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}