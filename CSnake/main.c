#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <WinUser.h>
#include <wingdi.h>
#include "main.h"

// global variables
int score = 0;
HBRUSH blackBrush;
HBRUSH whiteBrush;

struct snake {
    // [x,y]
    int curr[2];
};

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
    struct snake snake1 = { {50,50} };

    HDC hdc = GetDC(hwnd);
    // CreateSolidBrush(RGB(0, 0, 0));
    blackBrush = CreateSolidBrush(RGB(0,0,0));
    whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    while (TRUE) {
        // 624x421
        score++;
        //paintScore(hdc, score);

        // 0:up, 1:down, 2:left, 3:right
        snakeMove(hdc, &snake1, 1);

        Sleep(50);
    }
    DeleteObject(blackBrush);
    DeleteObject(whiteBrush);
    ReleaseDC(hwnd, hdc);
}

void snakeMove(HDC hdc, struct snake *snake1, int direction) {
    SelectObject(hdc, whiteBrush);
    drawRect(hdc, (*snake1).curr[0], (*snake1).curr[1]);

    switch (direction) {
        case 0:
            (*snake1).curr[1] -= 10;
            break;
        case 1:
            (*snake1).curr[1] += 10;
            break;
        case 2:
            (*snake1).curr[0] -= 10;
            break;
        case 3:
            (*snake1).curr[0] += 10;
            break;
    }


    SelectObject(hdc, blackBrush);
    drawRect(hdc, (*snake1).curr[0], (*snake1).curr[1]);
    Sleep(400);
}

void drawRect(HDC hdc, int x, int y) {
    // HDC, left, top, right, bottom
    Rectangle(hdc, x-6, y-6, x+6, y+6);
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
            printString(hdc, 5, 5, L"Score:");
            printNum(hdc, 53, 5, score);
            EndPaint(hdc, &ps);
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

    }
    // Does the default action for the message if undefined in the switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Paints the score after the score text
void paintScore(HDC hdc, int score) {
    printNum(hdc, 53, 5, score);
}

void printNum(HDC hdc, int x, int y, int num) {
    int len = numDigits(num);
    // Create an empty wide character array of length `len`
    wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * (int)log10(num)+10);
    // Copy int `num` into wide character array buffer `temp`
    swprintf_s(temp, sizeof(temp), L"%d", num);
    // Print text to window
    TextOut(hdc, x, y, temp, len);
    // Free the memory location of the wide character array
    free(temp);
}

void printString(HDC hdc, int x, int y, wchar_t string[]) {
    int len = wcslen(string);
    TextOut(hdc, x, y, string, len);
}

// it may LOOK unefficient but its the fastest method in C with the only use case of display resolutions
int numDigits(int n) {
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    return -1;
}