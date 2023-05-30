#ifndef MAIN_H
#define MAIN_H

// Need to define UNICODE to avoid LPCSTR being used instead of wchat_t
#ifndef UNICODE
#define UNICODE
#endif 

#define ID_QUIT 100
#define ID_ABOUT 101
#define ID_SETTINGS 102
#define WIDTH 640
#define HEIGHT 480

// Initializes the window procedure signature
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI gameLoop(HWND hwnd);

#endif