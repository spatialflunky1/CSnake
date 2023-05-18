#ifndef MAIN_H
#define MAIN_H

#include <Windows.h>

// Need to define UNICODE to avoid LPCSTR being used instead of wchat_t
#ifndef UNICODE
#define UNICODE
#endif 

// Initializes the window procedure signature
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnResize(HWND hwnd, UINT flag, int width, int height);

#endif