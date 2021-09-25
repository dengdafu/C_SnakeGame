#ifndef GAME_BASIC_STEPS_H
#define GAME_BASIC_STEPS_H

#include "GameState.h"

// initiate the game
// store number of screen rows in row
// store number of screen cols in col
void GameInit(struct GameState* gameStatePtr);

// Update the game
void GameUpdate(struct GameState* gameStatePtr, char command);

#endif