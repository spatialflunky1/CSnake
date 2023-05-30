#ifndef GAME_H
#define GAME_H

void changePos(struct snake* snake1, int direction, int index);
void snakeMove(HDC hdc, struct snake* snake1, HBRUSH whiteBrush, HBRUSH blackBrush, HPEN whitePen, HPEN blackPen, int direction);

#endif