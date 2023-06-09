#ifndef UTILS_H
#define UTILS_H

#define len(arr) sizeof(arr)/sizeof(arr[0])

struct snake {
    // [x,y]
    int** curr;
    int apple[2];
    HBRUSH brush;
    HPEN pen;
};

void drawRect(HDC hdc, int x, int y, int size, struct snake* snake1);
wchar_t* intToWchar_t(int num);
void printNum(HDC hdc, int x, int y, int num);
void printString(HDC hdc, int x, int y, wchar_t string[]);
void paintScore(HDC hdc, int score);
int numDigits(int n);
int** append(int** array, int rows, int* val);
int** create2dArrayofSize(int rows);

#endif