#ifndef MAIN_H
#define MAIN_H

// Need to define UNICODE to avoid LPCSTR being used instead of wchat_t
#ifndef UNICODE
#define UNICODE
#endif 

#define ID_QUIT 100
#define ID_ABOUT 101
#define ID_SETTINGS 102
#define BTN_OK 200
#define WIDTH 653
#define HEIGHT 485

DWORD WINAPI gameLoop(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcSettings(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif