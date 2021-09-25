#ifndef GAME_STATE_H
#define GAME_STATE_H
#define RANDOM -1

#include <stdbool.h>

struct SnakeSegment
{
    int row;
    int col;
    char symbol;
    struct SnakeSegment* nextSegment;
};

struct Food
{
    int row;
    int col;
    char symbol;
};

enum ActionState
{
    paused,
    running,
    end
};

struct GameState {
    int num_row, num_col;
    int point;
    enum ActionState actionState;
    struct SnakeSegment* snake;
    struct Food food;
};

void NewGameState(int num_row, int num_col, int seed, struct GameState* gameStatePtr);

bool InsertNewFood(struct GameState* gameStatePtr);

void DrawSnake(struct GameState* gameStatePtr);

void DrawFood(struct GameState* gameStatePtr);

void DrawPoint(struct GameState* gameStatePtr, bool clrFlag);

void DrawMessage(struct GameState* gameStatePtr, char msg[]);

void DestorySnake(struct SnakeSegment* snake);

#endif