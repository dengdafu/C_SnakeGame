#define PRIVATE static

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include "GameState.h"
#include "GameLogic.h"

PRIVATE void InsertNewSnake(struct GameState* gameStatePtr)
{
    gameStatePtr->snake = malloc(sizeof(struct SnakeSegment));
    if (gameStatePtr->snake == NULL)
    {
        endwin();
        exit(EXIT_FAILURE);
    }
    gameStatePtr->snake->row = rand() % gameStatePtr->num_row;
    gameStatePtr->snake->col = rand() % gameStatePtr->num_col;
    gameStatePtr->snake->symbol = '#';
    gameStatePtr->snake->nextSegment = NULL;
} 

bool InsertNewFood(struct GameState* gameStatePtr)
{
    int num_posn = gameStatePtr->num_row * gameStatePtr->num_col;
    int start_posn = rand() % num_posn;
    int i;
    div_t posn;
    for(i = 0; i < num_posn; i++)
    {
        posn = div((start_posn + i) % num_posn, gameStatePtr->num_col);
        if ((mvinch(posn.quot, posn.rem) & A_CHARTEXT) == ' ')
        {
            gameStatePtr->food.row = posn.quot;
            gameStatePtr->food.col = posn.rem;
            gameStatePtr->food.symbol = '$';
            return true;
        }
    }

    return false;
}

void NewGameState(int num_row, int num_col, int seed, struct GameState* gameStatePtr)
{
    gameStatePtr->num_row = num_row;
    gameStatePtr->num_col = num_col;
    gameStatePtr->point = 0;
    gameStatePtr->actionState = paused;

    if (seed == RANDOM)
    {
        srand(time(0));   
    }
    else
    {
        srand(seed);
    }

    InsertNewSnake(gameStatePtr);
    InsertNewFood(gameStatePtr);
}

void DrawSnake(struct GameState* gameStatePtr)
{
    struct SnakeSegment* snakeSeg = gameStatePtr->snake;
    for(;snakeSeg != NULL; snakeSeg = snakeSeg->nextSegment)
    {
        mvprintw(snakeSeg->row, snakeSeg->col, "%c", snakeSeg->symbol);
    }
}

void DrawFood(struct GameState* gameStatePtr)
{
    mvprintw(gameStatePtr->food.row, gameStatePtr->food.col, "%c", gameStatePtr->food.symbol);
}

void DrawPoint(struct GameState* gameStatePtr, bool clrFlag)
{
    if (clrFlag)
    {
        // "0123456"
        // "Point: "
        move(gameStatePtr->num_row + 1, 7);
        clrtoeol();
    }

    mvprintw(gameStatePtr->num_row + 1, 7, "%d", gameStatePtr->point);
}

void DrawMessage(struct GameState* gameStatePtr, char msg[])
{
    move(gameStatePtr->num_row + 2, 0);
    clrtoeol();
    printw(msg);
}

void DestorySnake(struct SnakeSegment* snake)
{
    struct SnakeSegment* curSeg = snake;
    struct SnakeSegment* nextSeg;

    for(;curSeg != NULL;)
    {
        nextSeg = curSeg->nextSegment;
        free(curSeg);
        curSeg = nextSeg;
    }
}