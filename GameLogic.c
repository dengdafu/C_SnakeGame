#define PRIVATE static

#include <stdlib.h>
#include <ncurses.h>
#include "GameLogic.h"

PRIVATE void RemoveTail(struct SnakeSegment* snake)
{
    struct SnakeSegment* scanner;
    for (scanner = snake; scanner->nextSegment != NULL; scanner = scanner->nextSegment)
    {
        if (scanner->nextSegment->nextSegment == NULL)
        {
            mvprintw(scanner->nextSegment->row, scanner->nextSegment->col, " ");
            free(scanner->nextSegment);
            scanner->nextSegment = NULL;
            break;
        }
    }
}

PRIVATE void MoveHead(struct SnakeSegment** snakePtr, int new_head_row, int new_head_col)
{
    struct SnakeSegment* new_head = malloc(sizeof(struct SnakeSegment));
    if (new_head == NULL)
    {
        DestorySnake(*snakePtr);
        endwin();
        exit(EXIT_FAILURE);
    }

    new_head->row = new_head_row;
    new_head->col = new_head_col;
    new_head->symbol = (*snakePtr)->symbol;
    mvprintw(new_head_row, new_head_col, "%c", new_head->symbol);
    (*snakePtr)->symbol = ((*snakePtr)->symbol == '^' || (*snakePtr)->symbol == 'v')? '|': '-';
    mvprintw((*snakePtr)->row, (*snakePtr)->col, "%c", (*snakePtr)->symbol);
    new_head->nextSegment = *snakePtr;
    *snakePtr = new_head;
}

PRIVATE bool WillSnakeDie(struct SnakeSegment* snake, int new_head_row, int new_head_col, int maxRow, int maxCol)
{
    // suffice to only check head
    // row check
    if (new_head_row < 0 || new_head_row >= maxRow)
    {
        return true;
    }
    
    // col check
    if (new_head_col < 0 || new_head_col >= maxCol)
    {
        return true;
    }

    // body check
    struct SnakeSegment* scanner;
    for(scanner = snake->nextSegment; scanner != NULL; scanner = scanner->nextSegment)
    {
        // scanner->nextSegment != NULL: cause tail will move
        if ((snake->row == scanner->row) && (snake->col == scanner->col) && (scanner->nextSegment != NULL))
        {
            return true;
        }
    }

    return false;
}

enum SnakeState MoveSnake(struct SnakeSegment** snakePtr, int foodRow, int foodCol, int maxRow, int maxCol)
{
    int new_head_row, new_head_col;
    switch((*snakePtr)->symbol)
    {
        case '^': // moving up
            new_head_row = (*snakePtr)->row - 1;
            new_head_col = (*snakePtr)->col;
            break;
        case 'v': // moving down
            new_head_row = (*snakePtr)->row + 1;
            new_head_col = (*snakePtr)->col;
            break;
        case '<': // moving left
            new_head_row = (*snakePtr)->row;
            new_head_col = (*snakePtr)->col - 1;
            break;
        case '>': // moving right
            new_head_row = (*snakePtr)->row;
            new_head_col = (*snakePtr)->col + 1;
            break;
    }

    // moving to the food position will definitely not lead to death
    if ((new_head_row == foodRow) && (new_head_col == foodCol))
    {
        MoveHead(snakePtr, new_head_row, new_head_col);
        return ate;
    }
    else if (WillSnakeDie(*snakePtr, new_head_row, new_head_col, maxRow, maxCol))
    {
        return dead;
    }
    else
    {
        MoveHead(snakePtr, new_head_row, new_head_col);
        RemoveTail(*snakePtr);
        return normal;
    }
}