#ifndef UTILS_H
#define UTILS_H

#define len(arr) sizeof(arr)/sizeof(arr[0])

struct snake {
    // [x,y]
    int** curr;
    HBRUSH brush;
    HPEN pen;
};

void drawRect(HDC hdc, int x, int y, struct snake* snake1);
void printNum(HDC hdc, int x, int y, int num);
void printString(HDC hdc, int x, int y, wchar_t string[]);
void paintScore(HDC hdc, int score);
int numDigits(int n);
int** append(int** array, int rows, int* val);
int** create2dArrayofSize(int rows);

#endif