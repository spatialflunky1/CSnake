#include <Windows.h>
#include "utils.h"
#include "game.h"

int snakeLength = 1;

void changePos(struct snake* snake1, int direction, int index) {
    // Sets every other value to the previous value
    if (index != 0) {
        (*snake1).curr[index][0] = (*snake1).curr[index - 1][0];
        (*snake1).curr[index][1] = (*snake1).curr[index - 1][1];
    }
    else {
        switch (direction) {
            // Client Window: 
            // X: 6-618, Y: 6-415
            // Score box: 
            // X:0-95, Y:0-35
        case 1:
            // 30 pixels away from the top to avoid touching score
            if ((*snake1).curr[0][1] > 6 && ((*snake1).curr[0][0] >= 95 || (*snake1).curr[0][1] > 35))
                (*snake1).curr[0][1] -= 15;
            break;
        case 2:
            // Usable window height is 421 pixels (-6=415)
            if ((*snake1).curr[0][1] < 415) (*snake1).curr[0][1] += 15;
            break;
        case 3:
            if ((*snake1).curr[0][0] > 6 && ((*snake1).curr[0][0] > 96 || (*snake1).curr[0][1] >= 35))
                (*snake1).curr[0][0] -= 15;
            break;
        case 4:
            // Usable window width is 624 pixels (-6=618)
            if ((*snake1).curr[0][0] < 618) (*snake1).curr[0][0] += 15;
            break;
        }
    }
}

void snakeMove(HDC hdc, struct snake* snake1, HBRUSH whiteBrush, HBRUSH blackBrush, HPEN whitePen, HPEN blackPen, int direction, int* increase) {
    // Increases the length of the snake
    if (*increase) {
        increaseSnakeLength(snake1, hdc);
        snakeLength++;
        *increase = 0;
    }

    // Erase all rects
    for (int i = snakeLength-1; i >= 0; i--) {
        (*snake1).brush = whiteBrush;
        (*snake1).pen = whitePen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], snake1);
    }
    // Change all rect positions
    for (int i = snakeLength-1; i >= 0; i--) {
        changePos(snake1, direction, i);
    }
    // Redraw all rects
    for (int i = snakeLength-1; i >= 0; i--) {
        (*snake1).brush = blackBrush;
        (*snake1).pen = blackPen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], snake1);
    }

    paintScore(hdc, snakeLength-1);
    Sleep(100);
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