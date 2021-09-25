#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "GameState.h"

enum SnakeState {
    normal,
    ate,
    dead
};

enum SnakeState MoveSnake(struct SnakeSegment** snakePtr, int foodRow, int foodCol, int maxRow, int maxCol);

#endif