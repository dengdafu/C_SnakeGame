#include <stdio.h>
#include <ncurses.h>
#include "GameState.h"
#include "GameBasicSteps.h"

int main()
{
    struct GameState gameState;
    GameInit(&gameState);

    char command;

    for(;;)
    {
        command = getch();
        GameUpdate(&gameState, command);
    }

    return 0;
}