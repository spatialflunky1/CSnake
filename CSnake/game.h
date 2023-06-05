#ifndef GAME_H
#define GAME_H

int changePos(struct snake* snake1, int direction, int index);
void setRandApple(struct snake* snake1);
int snakeMove(HDC hdc, struct snake* snake1, HBRUSH backgroundBrush, HBRUSH snakeBrush, HBRUSH appleBrush, HPEN backgroundPen, HPEN snakePen, HPEN applePen, int direction);
void increaseSnakeLength(struct snake* snake1, HDC hdc);

#endif