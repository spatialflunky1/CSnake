#include <Windows.h>
#include "utils.h"
#include "game.h"

int snakeLength = 1;

int changePos(struct snake* snake1, int direction, int index) {
    // Sets every other value to the previous value
    if (index != 0) {
        (*snake1).curr[index][0] = (*snake1).curr[index - 1][0];
        (*snake1).curr[index][1] = (*snake1).curr[index - 1][1];
    }
    else {
        switch (direction) {
            // Client Window: 
            // X: 6-631, Y: 6-420
            // Score box: 
            // X:0-95, Y:0-35
        case 1:
            // 30 pixels away from the top to avoid touching score
            if ((*snake1).curr[0][1] > 15 && ((*snake1).curr[0][0] >= 95 || (*snake1).curr[0][1] >= 35)) {
                (*snake1).curr[0][1] -= 15;
            }
            else return -1;
            break;
        case 2:
            // Usable window height is 421 pixels (-6=420)
            if ((*snake1).curr[0][1] < 420) {
                (*snake1).curr[0][1] += 15;
            }
            else return -1;
            break;
        case 3:
            if ((*snake1).curr[0][0] > 15 && ((*snake1).curr[0][0] >= 95 || (*snake1).curr[0][1] >= 35)) {
                (*snake1).curr[0][0] -= 15;
            }
            else return -1;
            break;
        case 4:
            // Usable window width is 624 pixels (-6=631)
            if ((*snake1).curr[0][0] < 631) {
                (*snake1).curr[0][0] += 15;
            }
            else return -1;
            break;
        }
    }
    return 0;
}

void setRandApple(struct snake *snake1) {
    int num = rand() % 631;
    while (num % 15 != 0 || num < 95) num = rand() % 631;
    (*snake1).apple[0] = num;

    num = rand() % 420;
    while (num % 15 != 0 || num < 35) num = rand() % 420;
    (*snake1).apple[1] = num;
}

int snakeMove(HDC hdc, struct snake* snake1, HBRUSH backgroundBrush, HBRUSH snakeBrush, HBRUSH appleBrush, HPEN backgroundPen, HPEN snakePen, HPEN applePen, int direction) {
    if ((*snake1).apple[0] == (*snake1).curr[0][0] && (*snake1).apple[1] == (*snake1).curr[0][1]) {
        (*snake1).brush = backgroundBrush;
        (*snake1).pen = backgroundPen;
        drawRect(hdc, (*snake1).apple[0], (*snake1).apple[1], 6, snake1);
        setRandApple(snake1);
        (*snake1).brush = appleBrush;
        (*snake1).pen = applePen;
        drawRect(hdc, (*snake1).apple[0], (*snake1).apple[1], 6, snake1);

        increaseSnakeLength(snake1, hdc);
        setRandApple(&snake1);
        snakeLength++;
    }
    
    // Draw apple
    (*snake1).brush = appleBrush;
    (*snake1).pen = applePen;
    drawRect(hdc, (*snake1).apple[0], (*snake1).apple[1], 6, snake1);

    // Erase all rects
    for (int i = snakeLength-1; i >= 0; i--) {
        (*snake1).brush = backgroundBrush;
        (*snake1).pen = backgroundPen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], 6, snake1);
    }
    // Change all rect positions
    for (int i = snakeLength-1; i >= 0; i--) {
        if (changePos(snake1, direction, i) == -1) return -1;
    }
    // Redraw all rects
    for (int i = snakeLength-1; i >= 0; i--) {
        (*snake1).brush = snakeBrush;
        (*snake1).pen = snakePen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], 6, snake1);
    }

    if (GetDCBrushColor(hdc) != RGB(0,0,0)) {
        SelectObject(hdc, backgroundBrush);
    }
    Rectangle(hdc, 0, 0, 84, 24);
    printString(hdc, 5, 5, L"Score:");
    paintScore(hdc, snakeLength-1);
    Sleep(100);
    return 0;
}

void increaseSnakeLength(struct snake* snake1, HDC hdc) {
    // Allocate memory for a temporary list of 2 nums
    int* newPoint = malloc(2*sizeof(int));
    // Stop if unable to allocate memory
    if (newPoint == NULL) {
        free(newPoint);
        return 0;
    }
    // Sets both values to 0 as the initial point does not matter
    memset(newPoint, 0, 2 * sizeof(int));
    int** temp = append((*snake1).curr, snakeLength, newPoint);
    // Frees the temporary memory
    free(newPoint);
    (*snake1).curr = temp;
    paintScore(hdc, snakeLength);
}