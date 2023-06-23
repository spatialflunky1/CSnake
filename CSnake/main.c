#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include <WinUser.h>
#include <wingdi.h>
#include "main.h"
#include "utils.h"
#include "game.h"

/*
global variables
*/

// Snake direction
int direction = 0;
int snakeLength = 1;

// Settings thread
DWORD threadIDSettings = 1;
HANDLE sThread;
int closeSettings = 0;
int repaint = 0;

// Game thread
DWORD threadID = 0;
HANDLE gameThread;

// Colors (snake,apple,background)
COLORREF colors[3] = { RGB(0, 0, 0) , RGB(255,0,0) ,RGB(255,255,255)};
// (sr,ar,br,sg,ag,bg,sb,ab,bb), index: 0-2 = red, 3-5 = blue, 6-8 = green 
HWND settingBoxes[9];

// Pens and brushes
HBRUSH snakeBrush;
HBRUSH backgroundBrush;
HBRUSH appleBrush;
HPEN snakePen;
HPEN backgroundPen;
HPEN applePen;
HPEN blackPen;

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

    gameThread = CreateThread(NULL, 0, gameLoop, hwnd, 0, &threadID);

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

    snakeBrush = CreateSolidBrush(colors[0]);
    appleBrush = CreateSolidBrush(colors[1]);
    backgroundBrush = CreateSolidBrush(colors[2]);
    snakePen = CreatePen(PS_SOLID, 0, colors[0]);
    applePen = CreatePen(PS_SOLID, 0, colors[1]);
    backgroundPen = CreatePen(PS_SOLID, 0, colors[2]);
    // Need an unchanging black pen for score border
    blackPen = CreatePen(PS_SOLID, 0, RGB(0,0,0));

    SetBkMode(hdc, TRANSPARENT);
    while (TRUE) {
        if (repaint) {
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
            repaint = 0;
        }
        if (snakeMove(hdc, &snake1, backgroundBrush, snakeBrush, appleBrush, backgroundPen, snakePen, applePen, blackPen, direction) == -1) break;
    }

    for (int i = 0; i < snakeLength; i++) {
        free(snake1.curr[i]);
    }
    free(snake1.curr);

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
        if (closeSettings) {
            closeSettings = 0;
            ExitThread(0);
        }
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
        FillRect(hdc, &ps.rcPaint, backgroundBrush);
        SelectObject(hdc, blackPen);
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
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | SS_ETCHEDHORZ,
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

            CreateWindow(
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"OK",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                301,         // x position 
                250,         // y position 
                75,          // Button width
                25,          // Button height
                settingsHwnd,     // Parent window
                (HMENU)BTN_OK,    // No menu.
                (HINSTANCE)GetWindowLongPtr(settingsHwnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.

            for (int i = 0; i < 3; i++) {
                wchar_t* r = intToWchar_t(GetRValue(colors[i]));
                wchar_t* g = intToWchar_t(GetGValue(colors[i]));
                wchar_t* b = intToWchar_t(GetBValue(colors[i]));
                settingBoxes[(i * 3)] = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", r, WS_CHILD | WS_VISIBLE | WS_BORDER, 40 + ((i % 3) * 127), 77, 70, 20, settingsHwnd, NULL, NULL, NULL);
                settingBoxes[(i * 3) + 1] = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", g, WS_CHILD | WS_VISIBLE | WS_BORDER, 40 + ((i % 3) * 127), 137, 70, 20, settingsHwnd, NULL, NULL, NULL);
                settingBoxes[(i * 3) + 2] = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", b, WS_CHILD | WS_VISIBLE | WS_BORDER, 40 + ((i % 3) * 127), 197, 70, 20, settingsHwnd, NULL, NULL, NULL);
                free(r);
                free(g);
                free(b);
            }

            ShowWindow(settingsHwnd, SW_SHOW);
            sThread = CreateThread(NULL, 0, settingsThread, settingsHwnd, 0, &threadIDSettings);
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
    if (uMsg == WM_COMMAND) {
        int wmId = HIWORD(wParam);
        switch (wmId) {
        case BN_CLICKED:
            int buttonId = LOWORD(wParam);
            switch (buttonId) {
            case BTN_OK:
                DeleteObject(snakeBrush);
                DeleteObject(backgroundBrush);
                DeleteObject(appleBrush);
                DeleteObject(snakePen);
                DeleteObject(backgroundPen);
                DeleteObject(applePen);
                int output[9];
                for (int i = 0; i < 9; i++) {
                    wchar_t temp[4];
                    GetWindowText(settingBoxes[i], temp, 4);
                    output[i] = wcstol(temp, NULL, 10);
                }
                for (int i = 0; i < 3; i++) {
                    colors[i] = RGB(output[(i * 3)], output[(i * 3) + 1], output[(i * 3) + 2]);
                }
                snakeBrush = CreateSolidBrush(colors[0]);
                appleBrush = CreateSolidBrush(colors[1]);
                backgroundBrush = CreateSolidBrush(colors[2]);
                snakePen = CreatePen(PS_SOLID, 0, colors[0]);
                applePen = CreatePen(PS_SOLID, 0, colors[1]);
                backgroundPen = CreatePen(PS_SOLID, 0, colors[2]);

                DestroyWindow(hwnd);
                closeSettings = 1;
                repaint = 1;
                return 0;
            }
        }
    }
    else if (uMsg == WM_PAINT) {
        HDC hdc = GetDC(hwnd);
        printString(hdc, 42, 34, L"Snake: ");
        printString(hdc, 171, 34, L"Apple: ");
        printString(hdc, 279, 34, L"Background: ");
        for (int i = 0; i < 3; i++) {
            printString(hdc, 10 + (i * 127), 80, L"R: ");
            printString(hdc, 10 + (i * 127), 140, L"G: ");
            printString(hdc, 10 + (i * 127), 200, L"B: ");
        }

        MoveToEx(hdc, 127, 281, NULL);
        LineTo(hdc, 127, -1);
        MoveToEx(hdc, 254, 281, NULL);
        LineTo(hdc, 254, -1);
    }
            
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}