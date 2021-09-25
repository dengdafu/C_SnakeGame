#define PRIVATE static

#include <ncurses.h>
#include <stdlib.h>

#include "GameBasicSteps.h"
#include "GameLogic.h"
#include "Config.h"

PRIVATE void DrawAll(struct GameState* gameStatePtr, char msg[])
{
    erase();

    int i;
    char boundary[gameStatePtr->num_col];
    for (i = 0; i < gameStatePtr->num_col; i++)
    {
        boundary[i] = '-';
    }
    mvprintw(gameStatePtr->num_row, 0, boundary);
    DrawSnake(gameStatePtr);
    DrawFood(gameStatePtr);
    mvprintw(gameStatePtr->num_row + 1, 0, "Point: ");
    DrawPoint(gameStatePtr, true);
    DrawMessage(gameStatePtr, msg);
}

void GameInit(struct GameState* gameStatePtr)
{
    int num_scr_row, num_scr_col;
    initscr(); // Initiate the screen.
    halfdelay(UPDATE_TIME);
	curs_set(0);
	noecho();
    getmaxyx(stdscr, num_scr_row, num_scr_col); // Put num of scr rows in row, columns in col.
    NewGameState(num_scr_row - 3, num_scr_col, RANDOM, gameStatePtr); // last row displays point and message, second last is a boundary line.

    DrawAll(gameStatePtr, MSG_PAUSE);
    refresh();
}

void GameReset(struct GameState* gameStatePtr)
{
    DestorySnake(gameStatePtr->snake);
    NewGameState(gameStatePtr->num_row, gameStatePtr->num_col, RANDOM, gameStatePtr);
    DrawAll(gameStatePtr, MSG_PAUSE);
}

void GameUpdate(struct GameState* gameStatePtr, char command)
{
    switch (command)
    {
        case 'q':
            DestorySnake(gameStatePtr->snake);
            endwin();
            exit(EXIT_SUCCESS);
        case 'r':
            GameReset(gameStatePtr);
            break;
        case 'p':
            gameStatePtr->actionState = paused;
            DrawMessage(gameStatePtr, MSG_PAUSE);
            break;
        case 'w':
            if (gameStatePtr->snake->symbol != 'v')
            {
                gameStatePtr->actionState = running;
                gameStatePtr->snake->symbol = '^';
                DrawMessage(gameStatePtr, MSG_RUNNING);
            }
            break;
        case 's':
            if (gameStatePtr->snake->symbol != '^')
            {
                gameStatePtr->actionState = running;
                gameStatePtr->snake->symbol = 'v';
                DrawMessage(gameStatePtr, MSG_RUNNING);
            }
            break;
        case 'a':
            if (gameStatePtr->snake->symbol != '>')
            {
                gameStatePtr->actionState = running;
                gameStatePtr->snake->symbol = '<';
                DrawMessage(gameStatePtr, MSG_RUNNING);
            }
            break;
        case 'd':
            if (gameStatePtr->snake->symbol != '<')
            {
                gameStatePtr->actionState = running;
                gameStatePtr->snake->symbol = '>';
                DrawMessage(gameStatePtr, MSG_RUNNING);
            }
            break;
        default:
            break;
    }

    if (gameStatePtr->actionState == running)
    {
        switch(MoveSnake(&gameStatePtr->snake, gameStatePtr->food.row, gameStatePtr->food.col, gameStatePtr->num_row, gameStatePtr->num_col))
        {
            case ate:
                gameStatePtr->point ++;
                DrawPoint(gameStatePtr, false);
                if (InsertNewFood(gameStatePtr) == false)
                {
                    gameStatePtr->actionState = end;
                    DrawMessage(gameStatePtr, MSG_WIN);
                }
                DrawFood(gameStatePtr);
                break;
            case dead:
                gameStatePtr->actionState = end;
                DrawMessage(gameStatePtr, MSG_OVER);
                break;
            default:
                DrawPoint(gameStatePtr, false);
                break;
        }
    }

    refresh();
}

