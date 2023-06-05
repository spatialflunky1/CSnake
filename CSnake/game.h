#ifndef GAME_H
#define GAME_H

void changePos(struct snake* snake1, int direction, int index);
void setRandApple(struct snake* snake1);
void snakeMove(HDC hdc, struct snake* snake1, HBRUSH whiteBrush, HBRUSH blackBrush, HBRUSH redBrush, HPEN whitePen, HPEN blackPen, HPEN redPen, int direction);
void increaseSnakeLength(struct snake* snake1, HDC hdc);

#endif