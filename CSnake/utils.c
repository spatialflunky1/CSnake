#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

void drawRect(HDC hdc, int x, int y, int size, struct snake* snake1) {
    // HDC, left, top, right, bottom
    SelectObject(hdc, (*snake1).brush);
    SelectObject(hdc, (*snake1).pen);
    Rectangle(hdc, x - size, y - size, x + size, y + size);
}

// Paints the score after the score text
void paintScore(HDC hdc, int score) {
    printNum(hdc, 53, 5, score);
}

wchar_t* intToWchar_t(int num) {
    int len = numDigits(num);
    // Create an empty wide character array of length `len`, if num==0 use 1 for log10 as 0 would make the function undefined
    wchar_t* temp = num != 0 ? (wchar_t*)malloc(sizeof(wchar_t) * (int)log10(num) + 16) : (wchar_t*)malloc(sizeof(wchar_t) * (int)log10(1) + 16);
    // Fix 'temp' may be '0' warning
    if (temp == 0) return;
    // Copy int `num` into wide character array buffer `temp`
    swprintf_s(temp, len + 1, L"%d", num);
    return temp;
}

void printNum(HDC hdc, int x, int y, int num) {
    int len = numDigits(num);
    // Convert num to wchar_t
    wchar_t* temp = intToWchar_t(num);
    // Print text to window
    TextOut(hdc, x, y, temp, len);
    // Free the memory location of the wide character array
    free(temp);
}

void printString(HDC hdc, int x, int y, wchar_t string[]) {
    size_t len = wcslen(string);
    // Fix 'possible loss of data converting size_t to int' warning
    if (len > INT_MAX) return;
    TextOut(hdc, x, y, string, (int)len);
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

// Creates an empty 2d array
int** create2dArrayofSize(int rows) {
    int** temp = malloc((rows + 1) * sizeof(int*));
    // Fix null pointer warning
    if (temp == NULL) return NULL;
    for (int i = 0; i < rows + 1; i++) temp[i] = malloc(2 * sizeof(int));
    return temp;
}

// Append to an int array
int** append(int** array, int rows, int* val) {
    int** temp = create2dArrayofSize(rows + 1);
    for (int i = 0; i < rows + 1; i++) {
        for (int j = 0; j < 2; j++) {
            if (i < rows) {
                temp[i][j] = array[i][j];
            }
            else {
                temp[i][j] = val[j];
            }
        }
    }
    return temp;
}