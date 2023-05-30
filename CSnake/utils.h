#ifndef UTILS_H
#define UTILS_H

struct snake {
    // [x,y]
    int curr[2][2];
    HBRUSH brush;
    HPEN pen;
};

void drawRect(HDC hdc, int x, int y, struct snake* snake1);
void printNum(HDC hdc, int x, int y, int num);
void printString(HDC hdc, int x, int y, wchar_t string[]);
void paintScore(HDC hdc, int score);
int numDigits(int n);

#endif