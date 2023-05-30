#include <Windows.h>
#include "utils.h"
#include "game.h"

void changePos(struct snake* snake1, int direction, int index) {
    if (index != 0) {
        (*snake1).curr[index][0] = (*snake1).curr[index - 1][0];
        (*snake1).curr[index][1] = (*snake1).curr[index - 1][1];
    }
    else {
        switch (direction) {
            // Client Window: 
            // X: 6-618, Y: 6-415
            // Box: 
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

void snakeMove(HDC hdc, struct snake* snake1, HBRUSH whiteBrush, HBRUSH blackBrush, HPEN whitePen, HPEN blackPen, int direction) {
    // Erase all rects
    for (int i = 1; i >= 0; i--) {
        (*snake1).brush = whiteBrush;
        (*snake1).pen = whitePen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], snake1);
    }
    // Change all rect positions
    for (int i = 1; i >= 0; i--) {
        changePos(snake1, direction, i);
    }
    // Redraw all rects
    for (int i = 1; i >= 0; i--) {
        (*snake1).brush = blackBrush;
        (*snake1).pen = blackPen;
        drawRect(hdc, (*snake1).curr[i][0], (*snake1).curr[i][1], snake1);
    }

    Sleep(100);
}